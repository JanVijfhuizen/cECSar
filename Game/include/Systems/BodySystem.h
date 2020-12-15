#pragma once
#include "ComponentSystem.h"
#include "Components/BodyComponent.h"
#include "Components/Transform.h"

namespace game
{
	class BodySystem final : public cecsar::ComponentSystem<BodyComponent, Transform>
	{
		void OnUpdate(utils::SparseSet<BodyComponent>&, utils::SparseSet<Transform>&) override;
	};
}