﻿#pragma once
#include "JobSystem.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Utils/QuadTree.h"

namespace game
{
	class TransformSystem;

	class CollisionSystem final : public JobSystem<Collider, Transform>
	{
	private:
		struct TransformBuffer
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

		void OnCollision(int32_t a, int32_t b);

		static bool IntersectsQuad(const Collider& collider, 
			const Transform& world, const utils::Quad& quad);

		static bool IntersectsOther(
			const Collider& aCollider, const Transform& aWorld,
			const Collider& bCollider, const Transform& bWorld);
	};
}
