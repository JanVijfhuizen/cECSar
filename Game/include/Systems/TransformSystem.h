#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"

namespace game
{
	class TransformSystem final : public cecsar::ComponentSystem<Transform>
	{
		void OnUpdate(utils::SparseSet<Transform>&) override;

		static float Sort(const Transform& transform, int32_t index);
	};
}
