#include <Modules/RenderModule.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

game::RenderModule::~RenderModule()
{
	const auto target = SDL_GetRenderTarget(renderer);
	if (target == renderTexture)
		SDL_SetRenderTarget(renderer, nullptr);
	SDL_DestroyTexture(renderTexture);
}

void game::RenderModule::PreRender() const
{
	SDL_SetRenderDrawColor(renderer,
		colorForegroundClear, colorForegroundClear, colorForegroundClear, 0);
	SDL_SetRenderTarget(renderer, renderTexture);

	SDL_SetRenderDrawColor(renderer,
		colorBackgroundClear, colorBackgroundClear, colorBackgroundClear, 0);
	SDL_RenderClear(renderer);
}

void game::RenderModule::PostRender() const
{
	SDL_SetRenderTarget(renderer, nullptr);

	SDL_Rect scaler;
	scaler.x = xOffset + SCREEN_WIDTH * (1 - zoom) / 2;
	scaler.y = yOffset + SCREEN_HEIGHT * (1 - zoom) / 2;
	scaler.w = SCREEN_WIDTH * zoom;
	scaler.h = SCREEN_HEIGHT * zoom;

	SDL_RenderCopyEx(renderer, renderTexture, nullptr, &scaler,
		degrees, nullptr, SDL_FLIP_NONE);
}

SDL_Texture* game::RenderModule::GetTexture(const std::string& path)
{
	const auto found = textureMap.find(path);
	if (found != textureMap.end())
		return found->second;

	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	return texture;
}

void game::RenderModule::Clear()
{
	for (const auto& texture : textureMap)
		SDL_DestroyTexture(texture.second);
	textureMap.clear();
}

void game::RenderModule::Initialize(cecsar::Cecsar& cecsar)
{
	// Simple nullcheck.
	if (window)
		return;

	window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	renderTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
}
