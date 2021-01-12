#pragma once
#include "ComponentSystem.h"
#include "Components/Leg.h"
#include "Components/Joint.h"
#include "Components/Transform.h"
#include "TransformSystem.h"
#include "Modules/TimeModule.h"

namespace game
{
	class LegSystem final : public cecsar::ComponentSystem<Leg, Joint, Transform>
	{
	private:
		cecsar::Cecsar* _cecsar = nullptr;
		TransformSystem* _transformSystem = nullptr;
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Leg>&, 
			utils::SparseSet<Joint>&, utils::SparseSet<Transform>&) override;
	};
}
