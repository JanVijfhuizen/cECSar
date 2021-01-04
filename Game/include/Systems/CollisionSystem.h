#pragma once
#include "JobSystem.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Utils/QuadTree.h"
#include "Utils/IObserver.h"

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

	class CollisionSystem final : public JobSystem<Collider, Transform>,
		public utils::ISubject<HitInfo>
	{
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

		~CollisionSystem();
		void Initialize(cecsar::Cecsar& cecsar) override;

		void OnUpdate(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) override;
		void UpdateBuffer(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) const;
		void FillQuadTree(utils::SparseSet<Collider>&) const;
		void IterateQuadTree(utils::SparseSet<Collider>&) const;

		static bool IntersectsQuad(const Collider& collider, 
			const Transform& world, const utils::Quad& quad);

		static bool IntersectsOther(int32_t a, int32_t b,
			const Collider& aCollider, const Transform& aWorld,
			const Collider& bCollider, const Transform& bWorld,
			HitInfo& aInfo, HitInfo& bInfo);
	};
}
