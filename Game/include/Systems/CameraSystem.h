#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Components/CameraFollowTarget.h"
#include "Modules/RenderModule.h"

namespace game
{
	class CameraSystem final : public cecsar::ComponentSystem<CameraFollowTarget, Transform>
	{
	private:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<CameraFollowTarget>&, utils::SparseSet<Transform>&) override;
	};
}
