#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/RenderSystem.h"
#include "Factories/BlockFactory.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// Setup cecsar.
	cecsar::CecsarInfo info;
	info.setCapacity = 10000;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	// Component sets.
	auto& transforms = cecsar.GetSet<game::Transform>();

	// Setup some nonsense to experiment with.
	const int32_t WIDTH = 80, HEIGHT = 120;
	const int32_t* ptrs = cecsar.AddEntity<game::BlockFactory>(WIDTH * HEIGHT);

	// Setup floor.
	for (int32_t y = 0; y < HEIGHT; ++y)
	{
		const int32_t* row = &ptrs[y * WIDTH];

		for (int32_t x = 0; x < WIDTH; ++x)
		{
			const int32_t* index = row + x;
			auto& transform = transforms[*index];

			transform.x = x * renderModule.DEFAULT_IMAGE_SIZE * renderModule.DEFAULT_IMAGE_UPSCALING;
			transform.y = y * renderModule.DEFAULT_IMAGE_SIZE * renderModule.DEFAULT_IMAGE_UPSCALING;

			transform.z = -0.02f * (x + y);
		}
	}
	delete [] ptrs;

	float f = 0;

	SDL_Event event;
	while(true)
	{
		f += .02f;

		while (SDL_PollEvent(&event) != 0)
			;

		renderModule.PreRender();

		// Some testing nonsense.
		renderModule.zColorFallof = 1.2f + sin(f);
		renderModule.zMod = 1.2f + cos(f);

		// Update systems.
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();
	}

	SDL_Quit();
	return 0;
}
