#pragma once
#include "SDL.h"
#include <unordered_map>
#include <IModule.h>

namespace jecs 
{
	class Cecsar;
}

namespace game
{
	class RenderModule final : public cecsar::IModule
	{
	public:
		const char* WINDOW_TITLE = "Game Prototype";
		const int32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

		float zoom = 1;
		float degrees = 0;
		float xOffset = 0, yOffset = 0;

		float xCameraOffset = 0, yCameraOffset = 0;

		float depth = 0;
		float minDepth = .2;
		float depthModifier = 1;

		int32_t colorForegroundClear = 0xff;
		int32_t colorBackgroundClear = 0x00;

		~RenderModule();

		void PreRender() const;
		void PostRender() const;

		SDL_Texture* GetTexture(const std::string& path);
		void Clear();

	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;

	private:
		std::unordered_map<std::string, SDL_Texture*> textureMap;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* renderTexture;
	};
}
