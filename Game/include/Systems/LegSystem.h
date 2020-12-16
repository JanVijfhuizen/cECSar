#pragma once
#include "ComponentSystem.h"
#include "Components/LegComponent.h"
#include "Components/Transform.h"
#include "Modules/TimeModule.h"
#include "Components/MovementComponent.h"

namespace game
{
	class LegSystem final : public cecsar::ComponentSystem<LegComponent, MovementComponent, Transform>
	{
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<LegComponent>&,
			utils::SparseSet<MovementComponent>&, utils::SparseSet<Transform>&) override;
	};
}
