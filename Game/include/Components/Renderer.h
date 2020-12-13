#pragma once
#include <SDL.h>
#include "Utils/Color.h"

namespace game
{
	class RenderSystem;

	struct Renderer final
	{
		friend RenderSystem;

		SDL_Texture* texture = nullptr;
		Color color{};

		int32_t xScale = 1;
		int32_t yScale = 1;

		float rotation = 0;

		// For animation purposes.
		int32_t index = 0;
		int32_t count = 1;

		SDL_RendererFlip flip = SDL_FLIP_NONE;

	private:
		float _renderPriority;
	};
}
