#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"

game::CollisionSystem::~CollisionSystem()
{
	delete _quadTree;
	delete[] _transformBuffer;
}

void game::CollisionSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_transformSystem = &cecsar.GetSystem<TransformSystem>();

	auto& renderModule = cecsar.GetModule<RenderModule>();

	const float w = renderModule.SCREEN_WIDTH;
	const float h = renderModule.SCREEN_HEIGHT;

	_quadTree = new utils::QuadTree(w, h);
	_transformBuffer = new TransformBuffer[cecsar.info.setCapacity];
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
		buffer.moved = delta > 1e-4;
		buffer.world = world;
	}
}

void game::CollisionSystem::FillQuadTree(utils::SparseSet<Collider>& colliders) const
{
	// Remove only the instances that are either invalid or have moved (too much).
	_quadTree->Iterate([this, &colliders](auto& nodes)
		{
			// List of nodes.
			for (int32_t i = nodes.size() - 1; i >= 0; --i)
			{
				// Instances for each (sub)node.
				auto& node = *nodes[i];
				auto& quad = node.quad;
				auto& vector = nodes[i]->instances;

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
						if (IntersectsQuad(collider, buffer.world, quad))
							continue;
					}

					// Remove entity.
					vector.erase(vector.begin() + j);
					buffer.sorted = false;
				}
			}
		});

	_quadTree->Clear(false);

	// Only push the instances that have changed their position within the tree.
	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		auto& collider = colliders[i];
		auto& buffer = _transformBuffer[dense[i]];
		if (buffer.sorted)
			continue;
		buffer.sorted = true;

		auto& world = _transformBuffer[dense[i]].world;

		// Push the colliders based on their positions.
		_quadTree->TryPush(i, [&collider, &world](
			const int32_t _, const utils::Quad& quad) 
			{
				return IntersectsQuad(collider, world, quad);
			});
	}
}

void game::CollisionSystem::IterateQuadTree(utils::SparseSet<Collider>& colliders) const
{
	_quadTree->Iterate([this, &colliders](auto& nodes)
	{
		// List of nodes.
		for (int32_t i = nodes.size() - 1; i >= 0; --i)
		{
			// Instances for each (sub)node.
			auto& aVector = nodes[i]->instances;
			for (int32_t j = aVector.size() - 1; j >= 0; --j)
			{
				// Information entity A.
				const int32_t aIndex = aVector[j];
				auto& aCollider = colliders.Get(aIndex);
				auto& aWorld = _transformBuffer[aIndex].world;

				// Check every other collidable entity.
				for (int32_t k = i - 1; k >= 0; --k)
				{
					auto& bVector = nodes[i]->instances;
					for (int32_t l = j - 1; l >= 0; --l)
					{
						// Information entity B.
						const int32_t bIndex = bVector[l];
						auto& bCollider = colliders.Get(bIndex);
						auto& bWorld = _transformBuffer[bIndex].world;

						// If the intersection check fails.
						if (!IntersectsOther(
							aCollider, aWorld, 
							bCollider, bWorld))
							continue;

						// Do the thing.
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

bool game::CollisionSystem::IntersectsOther(
	const Collider& aCollider, const Transform& aWorld,
	const Collider& bCollider, const Transform& bWorld)
{
	const float threshold = aCollider.radius / 2 + bCollider.radius / 2;
	return (aWorld.position - bWorld.position).Magnitude() < threshold;
}
