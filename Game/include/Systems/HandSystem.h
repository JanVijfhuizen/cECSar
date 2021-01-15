#pragma once
#include "ComponentSystem.h"
#include "Components/Hand.h"
#include "Components/Joint.h"
#include "Utils/IObserver.h"
#include "CollisionSystem.h"
#include "Components/Controller.h"

namespace game
{
	class HandSystem final : public cecsar::ComponentSystem<Hand, Controller, Joint>, 
		utils::IObserver<HitInfo>
	{
	private:
		cecsar::Cecsar* _cecsar = nullptr;

		utils::SparseSet<Hand>* _hands = nullptr;
		utils::SparseSet<Controller>* _controllers = nullptr;
		utils::SparseSet<Joint>* _joints = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;

		void OnUpdate(utils::SparseSet<Hand>&, 
			utils::SparseSet<Controller>&, utils::SparseSet<Joint>&) override;
		void OnNotify(const HitInfo& info) override;
	};
}
