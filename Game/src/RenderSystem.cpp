#include <Systems/RenderSystem.h>
#include "Modules/RenderModule.h"
#include <algorithm>
#include "Sorter.h"
#include <iostream>
#include "Modules/BufferModule.h"
#include "Systems/TransformSystem.h"

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

	_transformBuffer = cecsar.GetModule<BufferModule<Transform>>().buffer;
}

void game::RenderSystem::OnUpdate(utils::SparseSet<Renderer>& renderers)
{
	SortIndexes(renderers);

	Color c4Render;
	const auto p4Camera = _module->cameraPos.v4;
	auto& screenRenderer = _module->GetRenderer();
	const int32_t imageSize = _module->DEFAULT_IMAGE_SIZE;

	const auto dense = renderers.GetDenseRaw();
	for (int32_t i = renderers.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = _sortableInfo[i].index;
		auto& renderer = renderers[index];
		auto& transform = _transformBuffer->Get(dense[index]);

		// Calculate screenspace position.
		utils::Vector3 screenSpace;
		const auto worldPosition = TransformSystem::GetWorldPosition(transform);
		screenSpace.v4 = _mm_sub_ps(worldPosition.v4, p4Camera);

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

		const float rotation = renderer.rotation + TransformSystem::GetWorldRotation(transform);
		SDL_RenderCopyEx(&screenRenderer, renderer.texture,
			&srcRect, &dstRect,
			rotation, nullptr, renderer.flip);
	}
}

void game::RenderSystem::SortIndexes(utils::SparseSet<Renderer>& renderers) const
{
	const int32_t count = renderers.GetCount();
	const auto dense = renderers.GetDenseRaw();
	const auto last = _sortableInfo + count;

	int32_t n = 0;
	std::generate(_sortableInfo, last,
		[this, &n, dense]
		{
			auto& transform = _transformBuffer->Get(dense[n]);
			return RenderInfo(n++, TransformSystem::GetWorldPosition(transform).z);
		});

	std::sort(_sortableInfo, last,
		[](RenderInfo& a, RenderInfo& b)
		{
			return a.order > b.order;
		});
}
