#include <Systems/RenderSystem.h>

void game::RenderSystem::Initialize(cecsar::Cecsar& cecsar)
{
	
}

void game::RenderSystem::OnUpdate(
	utils::SparseSet<Renderer>& renderers, 
	utils::SparseSet<Transform>& transforms)
{
	const auto iterator = renderers.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& renderer = renderers[i];
		auto& transform = transforms[iterator[i]];
	}

	for (auto& renderer : renderers)
	{
		
	}
}