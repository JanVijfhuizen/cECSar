#pragma once
#include "ComponentSystem.h"
#include "Components/Controller.h"

namespace game
{
	class JobConverterModule;

	class ControllerSystem final : public cecsar::ComponentSystem<Controller>
	{
	private:
		Controller _playerController;
		JobConverterModule* _jobConverter = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Controller>&) override;
	};
}
