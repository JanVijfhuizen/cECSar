#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"

namespace game
{
	class TimeModule;

	class MovementSystem final : public cecsar::ComponentSystem<Transform, Controller, MovementComponent>
	{
	private:
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>&, utils::SparseSet<Controller>&,
			utils::SparseSet<MovementComponent>&) override;
	};
}
