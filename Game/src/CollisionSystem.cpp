#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"
#include "Visitors/QuadCollisionVisitor.h"
#include "Visitors/ShapeCollisionVisitor.h"

void game::CollisionSystem::NotifyCollisions()
{
	for (auto& hit : _hits)
		Notify(hit);
}

void game::CollisionSystem::DrawDebug() const
{
	auto& renderer = _renderModule->GetRenderer();
	const auto offset = _renderModule->cameraPos;

	_quadTree->Iterate([&renderer, &offset](auto& nodes, const int32_t anchor)
		{
			for (int32_t i = nodes.size() - 1; i >= anchor; --i)
			{
				SDL_SetRenderDrawColor(&renderer, 0xff, 0xff, 0xff, 0xff);

				auto& node = *nodes[i];
				auto& quad = node.quad;
				auto& pos = quad.pos;

				const float x = pos.x - offset.x;
				const float y = pos.y - offset.y;

				const float w = quad.width;
				const float h = quad.height;

				SDL_RenderDrawLine(&renderer, x, y, x + w, y);
				SDL_RenderDrawLine(&renderer, x + w, y, x + w, y + h);

				SDL_RenderDrawLine(&renderer, x + w, y + h, x, y + h);
				SDL_RenderDrawLine(&renderer, x, y + h, x, y);

				SDL_SetRenderDrawColor(&renderer, 0xff, 0x00, 0x00, 0x00);

				auto& vector = node.instances;
				const int32_t count = vector.size();
				for (int32_t j = count - 1; j >= 0; --j)
				{
					const int32_t xLine = x + (j + 1) * 4;
					SDL_RenderDrawLine(&renderer, xLine, y, xLine, y + 4);
				}
			}
		});
}

game::CollisionSystem::~CollisionSystem()
{
	delete _quadTree;
	delete[] _transformBuffer;
}

void game::CollisionSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
	_transformSystem = &cecsar.GetSystem<TransformSystem>();

	const int32_t size = 1600;
	_quadTree = new utils::QuadTree({-size / 2, -size / 2 }, size, size);
	_transformBuffer = new TransformBuffer[cecsar.info.setCapacity];
	_hits.reserve(cecsar.info.setCapacity * 2);
}

void game::CollisionSystem::OnUpdate(
	utils::SparseSet<Collider>& colliders, 
	utils::SparseSet<Transform>& transforms)
{
	UpdateBuffer(colliders, transforms);
	FillQuadTree(colliders);
	IterateQuadTree(colliders);
}

void game::CollisionSystem::UpdateBuffer(
	utils::SparseSet<Collider>& colliders, 
	utils::SparseSet<Transform>& transforms) const
{
	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];
		auto& local = transforms.Get(index);

		auto& buffer = _transformBuffer[index];
		auto world = _transformSystem->ToWorld(local);

		const float delta = (world.position - buffer.world.position).SquaredLength();
		buffer.moved = delta > 1e-6;
		buffer.world = world;
	}
}

void game::CollisionSystem::FillQuadTree(utils::SparseSet<Collider>& colliders) const
{
	// Remove the instances that are either invalid or have moved (too much).
	_quadTree->Iterate([this, &colliders](auto& nodes, const int32_t anchor)
		{
			for (int32_t i = nodes.size() - 1; i >= anchor; --i)
			{
				auto& node = *nodes[i];
				auto& quad = node.quad;
				auto& vector = node.instances;

				for (int32_t j = vector.size() - 1; j >= 0; --j)
				{
					const int32_t index = vector[j];
					auto& buffer = _transformBuffer[index];

					// Validate component, and try pushing it to a nested node.
					bool pushed = false;
					if (colliders.Contains(index))
					{
						// Check if moved.
						if (!buffer.moved)
							continue;

						// Despite being moved, check if it still part of this quad.
						auto& collider = colliders.Get(index);
						
						if(!node.IsLeaf())
						{
							pushed = _quadTree->TryPush(index, [&collider, &buffer](
								const int32_t _, const utils::Quad& quad)
								{
									return std::visit(QuadCollisionVisitor{
										buffer.world, quad }, collider.type);
								}, &node, true);
						}

						// If it doesn't have nested nodes or the pushing failed.
						if(!pushed)
							if(std::visit(QuadCollisionVisitor{buffer.world, quad },
								collider.type))
								continue;
					}

					// Remove entity.
					vector.erase(vector.begin() + j);
					buffer.sorted = pushed;
				}
			}
		});

	// Clear the empty nodes.
	_quadTree->Clear(false);

	// Only push the instances that have changed their position within the tree.
	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];

		auto& collider = colliders[i];
		auto& buffer = _transformBuffer[index];
		if (buffer.sorted)
			continue;

		auto& world = buffer.world;

		// Push the colliders based on their positions.
		buffer.sorted = _quadTree->TryPush(index, [&collider, &world](
			const int32_t _, const utils::Quad& quad) 
			{
				return std::visit(QuadCollisionVisitor{world, quad }, 
					collider.type);
			});
	}
}

void game::CollisionSystem::IterateQuadTree(utils::SparseSet<Collider>& colliders)
{
	_hits.clear();

	// Check for possible collisions.
	_quadTree->Iterate([this, &colliders](
		auto& nodes, const int32_t anchor)
	{
		// List of nodes.
		for (int32_t i = nodes.size() - 1; i >= anchor; --i)
		{
			auto& node = *nodes[i];
			auto& vector = node.instances;

			// Check collision for own instances.
			for (int32_t j = vector.size() - 1; j >= 0; --j)
			{
				const int32_t index = vector[j];
				auto& collider = colliders.Get(index);
				auto& world = _transformBuffer[index].world;

				for (int32_t k = j - 1; k >= 0; --k)
				{
					const int32_t otherIndex = vector[k];
					auto& otherCollider = colliders.Get(otherIndex);
					auto& otherWorld = _transformBuffer[otherIndex].world;

					CheckIntersection(index, otherIndex, collider, world,
						otherCollider, otherWorld);
				}
			}

			// Check instance collisions against other nodes.
			for (int32_t j = i - 1; j >= 0; --j)
			{
				auto& otherNode = *nodes[j];
				auto& otherVector = otherNode.instances;

				for (auto& index : vector)
				{
					auto& collider = colliders.Get(index);
					auto& world = _transformBuffer[index].world;

					for (auto& otherIndex : otherVector)
					{
						auto& otherCollider = colliders.Get(otherIndex);
						auto& otherWorld = _transformBuffer[otherIndex].world;

						CheckIntersection(index, otherIndex, collider, world,
							otherCollider, otherWorld);
					}
				}
			}
		}
	});
}

void game::CollisionSystem::CheckIntersection(const int32_t a, const int32_t b,
	const Collider& aCollider, const Transform& aWorld,
	const Collider& bCollider, const Transform& bWorld)
{
	// If neither masks collide, don't bother calculating.
	if ((aCollider.targetMask & bCollider.mask) == 0 &&
		(bCollider.targetMask & aCollider.mask) == 0)
		return;

	/*
	This looks bad (why not use inheritance?), but faking inheritance
	like this actually saves so much performance, and there are only going to be two
	shapes max.

	Collisions are the most performance heavy part of this program (at the time of writing),
	even after optimizing it quite a bit with a quadtree and minimizing pushing/clearing calls.
	*/

	utils::Vector3 aPoint, bPoint;
	utils::Vector3 aIntersect, bIntersect;

	CollisionInfo info{&aWorld, &bWorld, 
		&aPoint, &bPoint, 
		&aIntersect, &bIntersect};

	ShapeCollisionVisitor&& visitor = ShapeCollisionVisitor{ aCollider.type, info };
	const bool hit = std::visit(visitor, bCollider.type);
	if (!hit)
		return;

	// Parse collision information.
	HitInfo aInfo{};
	aInfo.point = aPoint;
	aInfo.intersection = aIntersect;

	HitInfo bInfo{};
	bInfo.point = bPoint;
	bInfo.intersection = bIntersect;

	// Parse instance information.
	auto& aInstance = aInfo.instance;
	auto& bInstance = bInfo.instance;

	aInstance.index = a;
	bInstance.index = b;

	aInstance.collider = &aCollider;
	bInstance.collider = &bCollider;

	aInstance.world = &aWorld;
	bInstance.world = &bWorld;

	// Copy instance info to other hit info.
	aInfo.other = bInstance;
	bInfo.other = aInstance;

	// Check masks before adding it to the hits.
	if ((aCollider.mask & bCollider.targetMask) != 0)
		_hits.push_back(aInfo);
	if ((bCollider.mask & aCollider.targetMask) != 0)
		_hits.push_back(bInfo);
}
