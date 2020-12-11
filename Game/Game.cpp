#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/RenderSystem.h"
#include "Factories/PlayerFactory.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	cecsar::Cecsar cecsar{};

	// Setup some nonsense to experiment with.
	delete [] cecsar.AddEntity<game::PlayerFactory>(200);

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
