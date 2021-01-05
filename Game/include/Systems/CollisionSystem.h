#pragma once
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Utils/QuadTree.h"
#include "Utils/IObserver.h"
#include "ComponentSystem.h"
#include "Modules/RenderModule.h"

namespace game
{
	class TransformSystem;

	struct HitInstance final
	{
		int32_t index{};
		const Collider* collider = nullptr;
		const Transform* world = nullptr;
	};

	struct HitInfo final
	{
		HitInstance instance{};
		HitInstance other{};

		utils::Vector3 point{};
	};

	class CollisionSystem final : public cecsar::ComponentSystem<Collider, Transform>,
		public utils::ISubject<HitInfo>
	{
	public:
		void NotifyCollisions();
		void Draw();

	private:
		struct TransformBuffer final
		{
			bool moved = false;
			bool sorted = false;
			Transform world{};
		};

		utils::QuadTree* _quadTree = nullptr;
		TransformSystem* _transformSystem = nullptr;
		TransformBuffer* _transformBuffer = nullptr;
		RenderModule* _renderModule = nullptr;

		std::vector<HitInfo> _hits{};

		~CollisionSystem();
		void Initialize(cecsar::Cecsar& cecsar) override;

		void OnUpdate(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) override;
		void UpdateBuffer(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) const;
		void FillQuadTree(utils::SparseSet<Collider>&) const;
		void IterateQuadTree(utils::SparseSet<Collider>&);

		static bool IntersectsQuad(const Collider& collider, 
			const Transform& world, const utils::Quad& quad);

		void CheckIntersection(int32_t a, int32_t b,
			const Collider& aCollider, const Transform& aWorld,
			const Collider& bCollider, const Transform& bWorld);
	};
}
