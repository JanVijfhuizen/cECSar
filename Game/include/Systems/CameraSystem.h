#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Components/CameraFollowTarget.h"
#include "Modules/RenderModule.h"
#include "Modules/TimeModule.h"
#include "Modules/BufferModule.h"

namespace game
{
	class CameraSystem final : public cecsar::ComponentSystem<CameraFollowTarget>
	{
	private:
		utils::Vector3 _followThreshold{80, 60, 0 };
		utils::Vector3  _hardFollowThreshold{160, 120, 0 };

		float _followSpeed = 100;

		float _movementZoomMultiplier = .2f;
		float _movementZoomThreshold = .2f;

		RenderModule* _renderModule = nullptr;
		TimeModule* _timeModule = nullptr;
		utils::SparseSet<Transform>* _transformBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<CameraFollowTarget>&) override;
		void UpdatePosition(const utils::Vector3& target) const;
	};
}
