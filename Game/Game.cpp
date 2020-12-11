#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	cecsar::Cecsar cecsar{};
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	SDL_Event event;
	while(true)
	{
		if (SDL_PollEvent(&event) != 0)
			;

		renderModule.PreRender();
		renderModule.PostRender();
	}

	SDL_Quit();
	return 0;
}
