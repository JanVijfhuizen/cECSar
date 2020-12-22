#include <Systems/RenderSystem.h>
#include "Modules/RenderModule.h"
#include <algorithm>
#include "Sorter.h"
#include <iostream>

game::RenderSystem::~RenderSystem()
{
	delete[] _sortableInfo;
}

game::RenderSystem::RenderInfo::RenderInfo() = default;

game::RenderSystem::RenderInfo::RenderInfo(const int32_t index, const float order) : 
	index(index), order(order)
{

}

void game::RenderSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_module = &cecsar.GetModule<RenderModule>();
	_sortableInfo = new RenderInfo[cecsar.info.setCapacity];
}

void game::RenderSystem::OnUpdate(
	utils::SparseSet<Renderer>& renderers, 
	utils::SparseSet<Transform>& transforms)
{
	SortIndexes(renderers, transforms);

	Color c4Render;
	const auto p4Camera = _module->cameraTransform.posLocal.v4;
	auto& screenRenderer = _module->GetRenderer();
	const int32_t imageSize = _module->DEFAULT_IMAGE_SIZE;

	const auto iterator = renderers.GetDenseIterator();
	for (int32_t i = renderers.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = _sortableInfo[i].index;
		auto& renderer = renderers[index];
		auto& transform = transforms.Get(iterator[index]);

		// Calculate screenspace position.
		utils::Vector3 screenSpace;
		screenSpace.v4 = _mm_sub_ps(transform.posGlobal.v4, p4Camera);

		SDL_Rect srcRect;
		srcRect.x = imageSize * renderer.index;
		srcRect.y = 0;
		srcRect.w = imageSize;
		srcRect.h = imageSize;

		// Spread objects out based on depth/z axis.
		const float depthModifier = std::max(.0f, 1.0f + screenSpace.z * _module->zMod);
		const float positionModifier = (1 - depthModifier) / 2;

		// Calculate screen position based on world position.
		SDL_Rect dstRect;
		const float imgMod = depthModifier * imageSize * _module->DEFAULT_IMAGE_UPSCALING;

		dstRect.w = renderer.xScale * imgMod;
		dstRect.h = renderer.yScale * imgMod;

		dstRect.x = (screenSpace.x - dstRect.w / 2) * depthModifier +
			positionModifier * _module->SCREEN_WIDTH;
		dstRect.y = (screenSpace.y - dstRect.h / 2) * depthModifier +
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
			(abs(screenSpace.z) - _module->zColorFallofThreshold) * _module->zColorFallof);
		c4Render.c4 = _mm_mul_ps(renderer.color.c4, _mm_set_ps1(colorMultiplier));

		SDL_SetTextureColorMod(renderer.texture, 
			c4Render.r, c4Render.g, c4Render.b);

		const float rotation = renderer.rotation + transform.rotGlobal;
		SDL_RenderCopyEx(&screenRenderer, renderer.texture,
			&srcRect, &dstRect,
			rotation, nullptr, renderer.flip);
	}
}

void game::RenderSystem::SortIndexes(
	utils::SparseSet<Renderer>& renderers, 
	utils::SparseSet<Transform>& transforms) const
{
	const int32_t count = renderers.GetCount();
	const auto iterator = renderers.GetDenseIterator();
	const auto last = _sortableInfo + count - 1;

	int32_t n = 0;
	std::generate(_sortableInfo, last,
		[&n, &iterator, &transforms]
		{
			return RenderInfo(n++, transforms.Get(iterator[n]).posGlobal.z);
		});

	std::sort(_sortableInfo, last,
		[](RenderInfo& a, RenderInfo& b)
		{
			return a.order > b.order;
		});
}
