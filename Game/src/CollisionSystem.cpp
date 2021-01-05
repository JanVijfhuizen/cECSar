#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"
#include <iostream>

void game::CollisionSystem::NotifyCollisions()
{
	for (auto& hit : _hits)
		Notify(hit);
}

void game::CollisionSystem::Draw()
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

	const int32_t size = 400;
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

					// Validate component.
					if (colliders.Contains(index))
					{
						// Check if moved.
						if (!buffer.moved)
							continue;

						// Despite being moved, check if it still part of this quad.
						auto& collider = colliders.Get(index);

						bool pushed = false;
						if(!node.IsLeaf())
						{
							pushed = _quadTree->TryPush(index, [&collider, &buffer](
								const int32_t _, const utils::Quad& quad)
								{
									return IntersectsQuad(
										collider, buffer.world, quad);
								}, &node, true);
						}

						// If it doesn't have nested nodes or the pushing failed.
						if(!pushed)
							if (IntersectsQuad(collider, buffer.world, quad))
								continue;
					}

					// Remove entity.
					vector.erase(vector.begin() + j);
					buffer.sorted = false;
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
				return IntersectsQuad(collider, world, quad);
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

bool game::CollisionSystem::IntersectsQuad(const Collider& collider,
	const Transform& world, const utils::Quad& quad)
{
	const auto& position = world.position;

	const float xCol = position.x;
	const float yCol = position.y;

	const float xQuad = quad.pos.x;
	const float yQuad = quad.pos.y;

	const float quadWidth = quad.width;
	const float quadHeight = quad.height;

	const float radiusHalf = collider.radius / 2;

	// Horizontal check.
	if (xCol - radiusHalf < xQuad ||
		yCol - radiusHalf < yQuad)
		return false;

	// Vertical check.
	if (xCol + radiusHalf >= xQuad + quadWidth ||
		yCol + radiusHalf >= yQuad + quadHeight)
		return false;

	return true;
}

void game::CollisionSystem::CheckIntersection(const int32_t a, const int32_t b,
	const Collider& aCollider, const Transform& aWorld,
	const Collider& bCollider, const Transform& bWorld)
{
	// If neither masks collide, don't bother calculating.
	if ((aCollider.targetMask & bCollider.mask) == 0 &&
		(bCollider.targetMask & aCollider.mask) == 0)
		return;

	// Circle collision.
	const float threshold = aCollider.radius / 2 + bCollider.radius / 2;
	const auto intersection = aWorld.position - bWorld.position;
	const float intMagnitude = intersection.Magnitude();

	// Check if in range.
	if (intMagnitude > threshold)
		return;

	// Set up hit info.
	HitInfo aInfo{};
	HitInfo bInfo{};
	
	aInfo.point = intersection;
	bInfo.point = intersection * -1;

	// Set up hit instances.
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
