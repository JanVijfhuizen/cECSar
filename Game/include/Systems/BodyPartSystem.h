#pragma once
#include "ComponentSystem.h"
#include "Components/BodyPart.h"
#include "Components/Transform.h"
#include "Components/Kinematic.h"

namespace game
{
	class BodyPartSystem final : public cecsar::ComponentSystem<BodyPart, Kinematic, Transform>
	{
	private:
		void OnUpdate(utils::SparseSet<BodyPart>&, 
			utils::SparseSet<Kinematic>&, utils::SparseSet<Transform>&) override;
	};
}
