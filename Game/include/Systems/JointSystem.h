#pragma once
#include "Components/Joint.h"
#include "Components/Transform.h"
#include "TransformSystem.h"
#include "Components/RigidBody.h"
#include "Modules/TimeModule.h"

namespace game
{
	class JointSystem final : public JobSystem<Joint, RigidBody, Transform>
	{
	public:
		float rotationSpeed = 500;

	private:
		cecsar::Cecsar* _cecsar = nullptr;
		TransformSystem* _transformSystem = nullptr;
		TimeModule* _timeModule = nullptr;
		
		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Joint>&, utils::SparseSet<RigidBody>&, utils::SparseSet<Transform>&) override;
	};
}
