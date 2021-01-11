#pragma once
#include "JobSystem.h"
#include "Components/Animator.h"
#include "Components/Renderer.h"
#include "Modules/TimeModule.h"

namespace game
{
	class AnimatorSystem final : public JobSystem<Animator, Renderer>
	{
	private:
		TimeModule* _timeModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Animator>&, utils::SparseSet<Renderer>&) override;
	};
}
