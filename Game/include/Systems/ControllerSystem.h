#pragma once
#include "ComponentSystem.h"
#include "Components/Controller.h"

namespace game
{
	class ControllerSystem final : public cecsar::ComponentSystem<Controller>
	{
	private:
		void OnUpdate(utils::SparseSet<Controller>&) override;
	};
}
