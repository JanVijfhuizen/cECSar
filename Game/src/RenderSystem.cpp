#include <Systems/RenderSystem.h>
#include "Modules/RenderModule.h"
#include <algorithm>

void game::RenderSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_module = &cecsar.GetModule<RenderModule>();
}

void game::RenderSystem::OnUpdate(
	utils::SparseSet<Renderer>& renderers, 
	utils::SparseSet<Transform>& transforms)
{
	const auto p4Camera = _module->transform.p4;

	// Used for SIMD calculations.
	Transform p4Screen;

	auto& screenRenderer = _module->GetRenderer();

	const auto iterator = renderers.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& renderer = renderers[i];
		auto& transform = transforms[iterator[i]];

		p4Screen.p4 = _mm_sub_ps(transform.p4, p4Camera);

		SDL_Rect srcRect;
		srcRect.x = 8 * renderer.index;
		srcRect.y = 0;
		srcRect.w = 8;
		srcRect.h = 8;

		// Spread objects out based on depth/z axis.
		const float depthModifier = std::max(.2f, p4Screen.z * _module->zMod);
		const float positionModifier = (1 - depthModifier) / 2;

		SDL_Rect dstRect;
		dstRect.w = renderer.xScale * depthModifier * 80;
		dstRect.h = renderer.yScale * depthModifier * 80;
		dstRect.x = p4Screen.x * depthModifier + positionModifier * _module->SCREEN_WIDTH;
		dstRect.y = p4Screen.y * depthModifier + positionModifier * _module->SCREEN_HEIGHT;

		const float rotation = renderer.rotation + transform.rotation;
		SDL_RenderCopyEx(&screenRenderer, renderer.texture,
			&srcRect, &dstRect,
			rotation, nullptr, renderer.flip);
	}
}