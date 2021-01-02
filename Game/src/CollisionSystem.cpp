#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"

game::CollisionSystem::~CollisionSystem()
{
	delete _quadTree;
	delete[] _worldBuffer;
}

void game::CollisionSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_transformSystem = &cecsar.GetSystem<TransformSystem>();

	auto& renderModule = cecsar.GetModule<RenderModule>();

	const float w = renderModule.SCREEN_WIDTH;
	const float h = renderModule.SCREEN_HEIGHT;

	_quadTree = new utils::QuadTree(w, h);
	_worldBuffer = new Transform[cecsar.info.setCapacity];
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
		_worldBuffer[index] = _transformSystem->ToWorld(local);
	}
}

void game::CollisionSystem::FillQuadTree(utils::SparseSet<Collider>& colliders) const
{
	_quadTree->Clear();

	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		auto& collider = colliders[i];

		// Push the colliders based on their positions.
		_quadTree->TryPush(i, [this, &collider](
			const int32_t index, const utils::Quad& quad) 
			{
				auto& buffer = _worldBuffer[index];
				return IntersectsQuad(collider, buffer, quad);
			});
	}
}

void game::CollisionSystem::IterateQuadTree(utils::SparseSet<Collider>& colliders) const
{
	_quadTree->Iterate([this, &colliders](auto& instances)
	{
		// List of instances that could collide with eachother.
		for (int32_t i = instances.size() - 1; i >= 0; --i)
		{
			// Instances for each (sub)node.
			auto& aVector = *instances[i];
			for (int32_t j = aVector.size() - 1; j >= 0; --j)
			{
				// Information entity A.
				const int32_t aIndex = aVector[j];
				auto& aCollider = colliders.Get(aIndex);
				auto& aWorld = _worldBuffer[aIndex];

				// Check every other collidable entity.
				for (int32_t k = i - 1; k >= 0; --k)
				{
					auto& bVector = *instances[i];
					for (int32_t l = j - 1; l >= 0; --l)
					{
						// Information entity B.
						const int32_t bIndex = bVector[l];
						auto& bCollider = colliders.Get(bIndex);
						auto& bWorld = _worldBuffer[bIndex];

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
