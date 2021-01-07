#pragma once
#include "ComponentSystem.h"
#include "Components/RigidBody.h"
#include "Utils/IObserver.h"
#include "CollisionSystem.h"
#include "Modules/TimeModule.h"

namespace game
{
	class RigidBodySystem final : 
		public cecsar::ComponentSystem<RigidBody, Transform>,
	    utils::IObserver<HitInfo>
	{
	public:
		float collisionForce = 10.0f;

	private:
		utils::SparseSet<RigidBody>* _rigidBodies = nullptr;
		TimeModule* _timeModule = nullptr;
		TransformSystem* _transformSystem = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<RigidBody>&, utils::SparseSet<Transform>&) override;
		void OnNotify(const HitInfo& info) override;
	};
}
