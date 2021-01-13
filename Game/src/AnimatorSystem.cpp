#include "Systems/AnimatorSystem.h"
#include "Modules/TimeModule.h"
#include "Visitors/AnimatorVisitor.h"

void game::AnimatorSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<Animator, Renderer>::Initialize(cecsar);
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::AnimatorSystem::OnUpdate(
	utils::SparseSet<Animator>& animators, 
	utils::SparseSet<Renderer>& renderers)
{
	const float deltaTime = _timeModule->GetDeltaTime();
	const auto dense = animators.GetDenseRaw();

	auto& jobModule = GetJobConvModule();
	jobModule.ToLinearJobs(animators.GetCount(), 
		[deltaTime, &animators, &renderers, &dense](const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				// Check if the animator is paused.
				auto& animator = animators[i];
				if (animator.paused)
					continue;

				// Get renderer.
				const int32_t index = dense[i];
				auto& renderer = renderers.Get(index);

				// Get info for the visitor.
				AnimatorVisitor::Info&& info
				{
					animator,
					renderer,
					deltaTime
				};

				// Handle animator types.
				AnimatorVisitor&& visitor(info);
				std::visit(visitor, animator.type);
			}
		});

	jobModule.Start();
	jobModule.Wait();
}
