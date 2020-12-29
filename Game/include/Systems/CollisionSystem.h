#pragma once
#include "JobSystem.h"
#include "Components/Transform.h"
#include "Components/Collider.h"

namespace game
{
	class CollisionSystem final : public JobSystem<Collider, Transform>
	{
	private:
		void OnUpdate(utils::SparseSet<Collider>&, utils::SparseSet<Transform>&) override;
	};
}
