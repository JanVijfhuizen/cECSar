#pragma once
#include "ComponentSystem.h"
#include "Components/BodyComponent.h"
#include "Components/Transform.h"
#include "Modules/TimeModule.h"

namespace game
{
	class BodySystem final : public cecsar::ComponentSystem<BodyComponent, Transform>
	{
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<BodyComponent>&, utils::SparseSet<Transform>&) override;
	};
}
