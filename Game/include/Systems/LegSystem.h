#pragma once
#include "Components/LegComponent.h"
#include "Components/Transform.h"
#include "Modules/TimeModule.h"
#include "Components/MovementComponent.h"
#include "JobSystem.h"

namespace game
{
	class LegSystem final : public JobSystem<LegComponent, MovementComponent, Transform>
	{
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<LegComponent>&,
			utils::SparseSet<MovementComponent>&, utils::SparseSet<Transform>&) override;
	};
}
