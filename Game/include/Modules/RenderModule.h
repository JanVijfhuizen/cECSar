#pragma once
#include "SDL.h"
#include <unordered_map>
#include <IModule.h>
#include <Components/Transform.h>

namespace game
{
	class RenderModule final : public cecsar::IModule
	{
	public:
		const int32_t DEFAULT_IMAGE_UPSCALING = 4;

		const char* WINDOW_TITLE = "Game Prototype";
		const int32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

		utils::Vector3 cameraPos{};
		float cameraRot = 0;

		utils::Vector3 screenPos{};
		float screenRot = 0;

		float zoom = 1;

		int32_t colorForegroundClear = 0xff;
		int32_t colorBackgroundClear = 0x00;

		float zMod = 1;
		float zColorFallofThreshold = 0;
		float zColorFallof = 1;

		SDL_Renderer& GetRenderer() const;

		~RenderModule();

		void PreRender() const;
		void PostRender() const;

		SDL_Texture* GetTexture(const std::string& path);
		void Clear();

	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;

	private:
		std::unordered_map<std::string, SDL_Texture*> textureMap;

		SDL_Window* _window = nullptr;
		SDL_Renderer* _renderer = nullptr;
		SDL_Texture* _renderTexture = nullptr;
	};
}
