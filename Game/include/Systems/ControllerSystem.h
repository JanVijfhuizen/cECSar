#pragma once
#include "Components/Controller.h"
#include "JobSystem.h"

namespace game
{
	class JobConverterModule;

	class ControllerSystem final : public JobSystem<Controller>
	{
	private:
		Controller _playerController;

		void OnUpdate(utils::SparseSet<Controller>&) override;
	};
}
