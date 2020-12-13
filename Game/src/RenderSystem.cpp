#include <Systems/RenderSystem.h>
#include "Modules/RenderModule.h"
#include <algorithm>

void game::RenderSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_module = &cecsar.GetModule<RenderModule>();
}

float game::RenderSystem::Sort(const Renderer& renderer, int32_t index)
{
	return renderer._renderPriority;
}

void game::RenderSystem::OnUpdate(
	utils::SparseSet<Renderer>& renderers, 
	utils::SparseSet<Transform>& transforms)
{
	const auto iterator = renderers.GetDenseIterator();

	// Sort renderers based on the transforms z positions.
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& renderer = renderers[i];
		auto& transform = transforms.Get(iterator[i]);

		renderer._renderPriority = -transform.zGlobal;
	}
	renderers.Sort(Sort);

	// Used for calculations.
	Transform p4Screen;
	Color c4Render;
	const auto p4Camera = _module->transform.p4;
	auto& screenRenderer = _module->GetRenderer();
	const int32_t imageSize = _module->DEFAULT_IMAGE_SIZE;

	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& renderer = renderers[i];
		auto& transform = transforms.Get(iterator[i]);

		p4Screen.p4 = _mm_sub_ps(transform.p4Global, p4Camera);

		SDL_Rect srcRect;
		srcRect.x = imageSize * renderer.index;
		srcRect.y = 0;
		srcRect.w = imageSize;
		srcRect.h = imageSize;

		// Spread objects out based on depth/z axis.
		const float depthModifier = std::max(.0f, 1.0f + p4Screen.z * _module->zMod);
		const float positionModifier = (1 - depthModifier) / 2;

		// Calculate screen position based on world position.
		SDL_Rect dstRect;
		const float imgMod = depthModifier * imageSize * _module->DEFAULT_IMAGE_UPSCALING;

		dstRect.w = renderer.xScale * imgMod;
		dstRect.h = renderer.yScale * imgMod;

		dstRect.x = (p4Screen.x - dstRect.w / 2) * depthModifier + 
			positionModifier * _module->SCREEN_WIDTH;
		dstRect.y = (p4Screen.y - dstRect.h / 2) * depthModifier + 
			positionModifier * _module->SCREEN_HEIGHT;

		// Out of bounds check.
		if (dstRect.x > _module->SCREEN_WIDTH)
			continue;
		if (dstRect.y > _module->SCREEN_HEIGHT)
			continue;
		if (dstRect.x + dstRect.w < 0)
			continue;
		if (dstRect.y + dstRect.h < 0)
			continue;

		// Adjust color.
		const float colorMultiplier = 1.0f - std::max(.0f, 
			(abs(p4Screen.z) - _module->zColorFallofThreshold) * _module->zColorFallof);
		c4Render.c4 = _mm_mul_ps(renderer.color.c4, _mm_set_ps1(colorMultiplier));

		SDL_SetTextureColorMod(renderer.texture, 
			c4Render.r, c4Render.g, c4Render.b);

		const float rotation = renderer.rotation + transform.rotationGlobal;
		SDL_RenderCopyEx(&screenRenderer, renderer.texture,
			&srcRect, &dstRect,
			rotation, nullptr, renderer.flip);
	}
}
