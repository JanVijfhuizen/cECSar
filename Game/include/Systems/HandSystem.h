#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Components/HandComponent.h"

namespace game
{
	class HandSystem final : public cecsar::ComponentSystem<HandComponent, Transform>
	{
	private:
		void OnUpdate(utils::SparseSet<HandComponent>&, utils::SparseSet<Transform>&) override;
	};
}
