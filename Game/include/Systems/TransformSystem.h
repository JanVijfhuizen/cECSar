#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"

namespace game
{
	class TransformSystem final : public cecsar::ComponentSystem<Transform>
	{
	private:
		void OnUpdate(utils::SparseSet<Transform>&) override;
	};
}
