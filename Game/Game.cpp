#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/RenderSystem.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	cecsar::Cecsar cecsar{};

	// Modules.
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	SDL_Event event;
	while(true)
	{
		while (SDL_PollEvent(&event) != 0)
			;

		renderModule.PreRender();

		// Update systems.
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();
	}

	SDL_Quit();
	return 0;
}
