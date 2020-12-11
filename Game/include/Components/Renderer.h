#pragma once
#include <SDL.h>

namespace game
{
	struct Renderer final
	{
		SDL_Texture* texture = nullptr;

		int32_t xScale = 1;
		int32_t yScale = 1;

		float rotation = 0;

		int32_t xOffset = 0;
		int32_t yOffset = 0;

		// For animation purposes.
		int32_t index = 0;
		int32_t count = 1;

		bool xCenter = true;
		bool yCenter = true;

		SDL_RendererFlip flip = SDL_FLIP_NONE;
	};
}
