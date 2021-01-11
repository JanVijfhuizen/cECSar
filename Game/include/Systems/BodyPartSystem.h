#pragma once
#include "ComponentSystem.h"
#include "Components/BodyPart.h"
#include "Components/Transform.h"
#include "Components/Kinematic.h"
#include "TransformSystem.h"

namespace game
{
	class BodyPartSystem final : public cecsar::ComponentSystem<BodyPart, Kinematic, Transform>
	{
	private:
		cecsar::Cecsar* _cecsar = nullptr;
		TransformSystem* _transformSystem = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<BodyPart>&, 
			utils::SparseSet<Kinematic>&, utils::SparseSet<Transform>&) override;
	};
}
