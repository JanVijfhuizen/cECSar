#pragma once
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "JobSystem.h"

namespace game
{
	class TimeModule;

	class MovementSystem final : public JobSystem<MovementComponent, Transform>
	{
	private:
		TimeModule* _timeModule = nullptr;
		utils::SparseSet<Controller>* _controllerBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<MovementComponent>&, 
			utils::SparseSet<Transform>&) override;
	};
}
