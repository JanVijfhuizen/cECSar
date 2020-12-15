#include <Modules/RenderModule.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Renderer& game::RenderModule::GetRenderer() const
{
	return *_renderer;
}

game::RenderModule::~RenderModule()
{
	const auto target = SDL_GetRenderTarget(_renderer);
	if (target == _renderTexture)
		SDL_SetRenderTarget(_renderer, nullptr);

	SDL_DestroyTexture(_renderTexture);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

void game::RenderModule::PreRender() const
{
	SDL_SetRenderDrawColor(_renderer,
		colorForegroundClear, colorForegroundClear, colorForegroundClear, 0);
	SDL_SetRenderTarget(_renderer, _renderTexture);

	SDL_SetRenderDrawColor(_renderer,
		colorBackgroundClear, colorBackgroundClear, colorBackgroundClear, 0);
	SDL_RenderClear(_renderer);
}

void game::RenderModule::PostRender() const
{
	SDL_SetRenderTarget(_renderer, nullptr);

	SDL_Rect scaler;
	scaler.x = screenTransform.posLocal.x + SCREEN_WIDTH * (1 - zoom) / 2;
	scaler.y = screenTransform.posLocal.y + SCREEN_HEIGHT * (1 - zoom) / 2;
	scaler.w = SCREEN_WIDTH * zoom;
	scaler.h = SCREEN_HEIGHT * zoom;

	SDL_RenderCopyEx(_renderer, _renderTexture, nullptr, &scaler,
		screenTransform.rot, nullptr, SDL_FLIP_NONE);

	SDL_RenderPresent(_renderer);
}

SDL_Texture* game::RenderModule::GetTexture(const std::string& path)
{
	const auto found = textureMap.find(path);
	if (found != textureMap.end())
		return found->second;

	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

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
	if (_window)
		return;

	_window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	_renderTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
}
