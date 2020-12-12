#include <Systems/RenderSystem.h>
#include "Modules/RenderModule.h"
#include <algorithm>

game::RenderSystem::RendererSorter::RendererSorter(utils::SparseSet<Transform>& set) :
	_set(set)
{

}

bool game::RenderSystem::RendererSorter::operator()(const int32_t a, const int32_t b) const
{
	return _set[a].z > _set[b].z;
}

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
	const int32_t imageSize = _module->DEFAULT_IMAGE_SIZE;

	// Sort renderers based on z positions.
	const auto iterator = renderers.GetDenseIterator();
	std::sort(iterator.begin(), iterator.end(), RendererSorter(transforms));

	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& renderer = renderers[i];
		auto& transform = transforms[iterator[i]];

		p4Screen.p4 = _mm_sub_ps(transform.p4, p4Camera);

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
		const float imgMod = depthModifier * imageSize;

		dstRect.w = renderer.xScale * imgMod * _module->DEFAULT_IMAGE_UPSCALING;
		dstRect.h = renderer.yScale * imgMod * _module->DEFAULT_IMAGE_UPSCALING;

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
		const float colMultiplier = 1.0f - std::max(.0f, 
			(abs(p4Screen.z) - _module->zColorFallofThreshold) * _module->zColorFallof);
		const float colorMultiplier = 255 * colMultiplier;
		SDL_SetTextureColorMod(renderer.texture, 
			colorMultiplier, colorMultiplier, colorMultiplier);

		const float rotation = renderer.rotation + transform.rotation;
		SDL_RenderCopyEx(&screenRenderer, renderer.texture,
			&srcRect, &dstRect,
			rotation, nullptr, renderer.flip);
	}
}