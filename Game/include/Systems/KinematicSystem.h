#pragma once
#include "ComponentSystem.h"
#include "Components/Kinematic.h"
#include "Components/Transform.h"
#include "TransformSystem.h"

namespace game
{
	class KinematicSystem final : public cecsar::ComponentSystem<Kinematic, Transform>
	{
	private:
		cecsar::Cecsar* _cecsar = nullptr;
		TransformSystem* _transformSystem = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Kinematic>&, utils::SparseSet<Transform>&) override;
	};
}
