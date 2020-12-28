#pragma once
#include <EntityFactory.h>
#include <Factories/Environment/GroundBlockFactory.h>

namespace game
{
	class EnvironmentFactory final : public cecsar::EntityFactory
	{
	protected:
		inline void OnConstruction(
			cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info) override;
	};

	inline void EnvironmentFactory::OnConstruction(
		cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
	{
		const int32_t width = 32, height = 32;
		const auto blocks = cecsar.AddEntity<GroundBlockFactory>(width * height);

		auto& transforms = cecsar.GetSet<Transform>();
		auto& renderers = cecsar.GetSet<Renderer>();
		auto& renderModule = cecsar.GetModule<RenderModule>();

		auto& renderer = renderers.Get(blocks[0].index);

		const int32_t padding = 1;
		int32_t h;

		SDL_QueryTexture(renderer.texture, 
			nullptr, nullptr, nullptr, &h);
		
		h += padding;
		h *= renderModule.DEFAULT_IMAGE_UPSCALING;

		for (int32_t y = 0; y < height; ++y)
		{
			const auto row = blocks.get() + y * width;
			const float yPos = y * h;

			for (int32_t x = 0; x < width; ++x)
			{
				const int32_t blockIndex = row[x].index;
				auto& transform = transforms.Get(blockIndex);

				const float xPos = x * h;
				transform.position = {xPos, yPos, 0 };
			}
		}

		// This is just a proxy entity.
		cecsar.RemoveEntity(info.index);
	}
}
