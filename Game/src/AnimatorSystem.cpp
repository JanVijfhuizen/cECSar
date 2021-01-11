#include "Systems/AnimatorSystem.h"
#include "Modules/TimeModule.h"

void game::AnimatorSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::AnimatorSystem::OnUpdate(
	utils::SparseSet<Animator>& animators, 
	utils::SparseSet<Renderer>& renderers)
{
	const float deltaTime = _timeModule->GetDeltaTime();

	const auto dense = animators.GetDenseRaw();
	for (int32_t i = animators.GetCount() - 1; i >= 0; --i)
	{
		// Check if the animator is paused.
		auto& animator = animators[i];
		if (animator.paused)
			continue;

		// Increment lerp, and check if it finished the animation.
		animator.lerp += deltaTime;
		if (animator.lerp > 1 && !animator.repeat) 
		{
			animator.paused = true;
		}

		animator.lerp = fmod(animator.lerp, 1.0f);

		const int32_t index = dense[i];
		auto& renderer = renderers.Get(index);

		renderer.
	}
}
