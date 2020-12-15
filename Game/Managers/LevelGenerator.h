#pragma once
#include <cstdint>
#include "Factories/BlockFactory.h"

namespace cecsar 
{
	class Cecsar;
}

namespace game
{
	struct LevelGeneratorInfo final
	{
		int32_t width = 12;
		int32_t height = 8;
	};

	class LevelGenerator final
	{
	public:
		const int32_t* Generate(cecsar::Cecsar& cecsar, LevelGeneratorInfo info = LevelGeneratorInfo()) const;
	};

	inline const int32_t* LevelGenerator::Generate(cecsar::Cecsar& cecsar, const LevelGeneratorInfo info) const
	{
		// Modules.
		auto& renderModule = cecsar.GetModule<RenderModule>();

		// Component sets.
		auto& transforms = cecsar.GetSet<Transform>();
		auto& renderers = cecsar.GetSet<Renderer>();

		const int32_t* ptrs = cecsar.AddEntity<BlockFactory>(info.width * info.height);

		const int32_t posMod = renderModule.DEFAULT_IMAGE_SIZE * renderModule.DEFAULT_IMAGE_UPSCALING;
		const int32_t padding = renderModule.DEFAULT_IMAGE_SIZE / 2;

		// Setup floor.
		for (int32_t y = 0; y < info.height; ++y)
		{
			const int32_t* row = &ptrs[y * info.width];

			for (int32_t x = 0; x < info.width; ++x)
			{
				const int32_t* index = row + x;
				auto& transform = transforms[*index];

				transform.posLocal.x = x * posMod + x * padding;
				transform.posLocal.y = y * posMod + y * padding;

				//transform.posLocal.z = -.2f * (x + y);
				//transform.rot = x + y;

				auto& renderer = renderers[*index];
				renderer.color.r = x * 10;
				renderer.color.g = y * 10;
				renderer.color.b = abs(transform.posLocal.z) * 10;
			}
		}

		return ptrs;
	}
}
