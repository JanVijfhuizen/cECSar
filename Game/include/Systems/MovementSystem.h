#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "Modules/JobConverterModule.h"

namespace game
{
	class TimeModule;

	class MovementSystem final : public cecsar::ComponentSystem<MovementComponent, Transform>
	{
	private:
		TimeModule* _timeModule = nullptr;
		JobConverterModule* _jobConverter = nullptr;

		utils::SparseSet<Controller>* _controllerBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<MovementComponent>&, 
			utils::SparseSet<Transform>&) override;
	};
}
