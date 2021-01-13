#pragma once
#include "Components/Joint.h"
#include "Components/Transform.h"
#include "TransformSystem.h"

namespace game
{
	class JointSystem final : public JobSystem<Joint, Transform>
	{
	private:
		struct JointDelta final
		{
			int32_t index = -1;
			utils::Vector3 value{};
		};

		cecsar::Cecsar* _cecsar = nullptr;
		TransformSystem* _transformSystem = nullptr;
		std::vector<JointDelta> _deltas{};

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Joint>&, utils::SparseSet<Transform>&) override;
	};
}
