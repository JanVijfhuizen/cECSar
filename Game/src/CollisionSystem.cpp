#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"

game::CollisionSystem::~CollisionSystem()
{
	delete _quadTree;
}

void game::CollisionSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_transformSystem = &cecsar.GetSystem<TransformSystem>();

	auto& renderModule = cecsar.GetModule<RenderModule>();

	const float w = renderModule.SCREEN_WIDTH;
	const float h = renderModule.SCREEN_HEIGHT;
	_quadTree = new utils::QuadTree<Collider>(w, h);
}

void game::CollisionSystem::OnUpdate(
	utils::SparseSet<Collider>& colliders, 
	utils::SparseSet<Transform>& transforms)
{
	auto& jobModule = GetJobModule();

	_quadTree->Clear();

	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		auto& local = transforms.Get(dense[i]);
		auto world = _transformSystem->ToWorld(local);
		auto& collider = colliders[i];

		_quadTree->TryPush(collider,
			[&world](const Collider& collider, const utils::Quad& quad)
		{
			const auto& position = world.position;

			const float xCol = position.x;
			const float yCol = position.y;

			const float xQuad = quad.pos.x;
			const float yQuad = quad.pos.y;

			const float quadWidth = quad.width;
			const float quadHeight = quad.height;

			// Switch collision type.
			switch (collider.type)
			{
			case ColliderType::Circle:
				float radiusHalf;
				radiusHalf = collider.circle.radius / 2;

				// Horizontal check.
				if (xCol - radiusHalf < xQuad || 
					yCol - radiusHalf < yQuad)
					return false;

				// Vertical check.
				if (xCol + radiusHalf >= xQuad + quadWidth || 
					yCol + radiusHalf >= yQuad + quadHeight)
					return false;

				return true;

			case ColliderType::Rectangle:
				throw std::exception("Not implemented exception");

			default:
				break;
			}

			return true;
		});
	}
}
