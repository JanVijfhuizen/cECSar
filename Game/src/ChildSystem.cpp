#include <Systems/ChildSystem.h>

void game::ChildSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<ChildComponent, Transform>::Initialize(cecsar);

	_children = &cecsar.GetSet<ChildComponent>();
	_transforms = &cecsar.GetSet<Transform>();
}

void game::ChildSystem::OnUpdate(
	utils::SparseSet<ChildComponent>& children, 
	utils::SparseSet<Transform>&)
{
	const auto dense = children.GetDenseRaw();
	auto& jobModule = GetJobModule();

	jobModule.ToLinearJobs(children.GetCount(),
		[this, &dense]
		(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
				UpdateManually(dense[i]);
		});

	jobModule.Wait();

	// Remove all child components from objects that don't have parents anymore.
	for (auto index : _removables)
		children.RemoveAt(index);
}
