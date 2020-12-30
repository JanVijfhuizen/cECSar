#pragma once
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
		utils::QuadTree<Collider>* _quadTree = nullptr;
		TransformSystem* _transformSystem = nullptr;

		~CollisionSystem();
		void Initialize(cecsar::Cecsar& cecsar) override;

		void OnUpdate(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) override;
	};
}
