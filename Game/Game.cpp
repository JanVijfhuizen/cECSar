#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/TransformSystem.h"
#include "Systems/RenderSystem.h"
#include "Factories/BlockFactory.h"
#include "Helpers/TransformHelper.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// Setup cecsar.
	cecsar::CecsarInfo info;
	info.setCapacity = 1000;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& renderModule = cecsar.GetModule<game::RenderModule>();
	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Component sets.
	auto& transforms = cecsar.GetSet<game::Transform>();

	// Setup some nonsense to experiment with.
	const int32_t WIDTH = 20, HEIGHT = 12;
	const int32_t* ptrs = cecsar.AddEntity<game::BlockFactory>(WIDTH * HEIGHT);

	const int32_t posMod = renderModule.DEFAULT_IMAGE_SIZE * renderModule.DEFAULT_IMAGE_UPSCALING;
	const int32_t padding = renderModule.DEFAULT_IMAGE_SIZE / 2;

	// Setup floor.
	for (int32_t y = 0; y < HEIGHT; ++y)
	{
		const int32_t* row = &ptrs[y * WIDTH];

		for (int32_t x = 0; x < WIDTH; ++x)
		{
			const int32_t* index = row + x;
			auto& transform = transforms[*index];

			transform.x = x * posMod + x * padding;
			transform.y = y * posMod + y * padding;

			transform.z = -.2f * (x + y);
			transform.rotation = x + y;
		}
	}
	delete [] ptrs;

	const int32_t* ptrsMoving = cecsar.AddEntity<game::BlockFactory>(2);
	game::TransformHelper::SetParent(transforms, ptrsMoving[1], ptrsMoving[0]);
	//delete[] ptrsMoving;
	transforms[ptrsMoving[0]].y = 100;

	float f = 0;

	SDL_Event event;
	while(true)
	{
		// Deltatime? What's that?
		f += .02f;

		while (SDL_PollEvent(&event) != 0)
			;

		transforms[ptrsMoving[0]].x = 200 + sin(f) * 100;
		//transforms[ptrsMoving[1]].y = cos(f) * 100;

		renderModule.PreRender();
		cecsar.Update<game::RenderSystem>();
		renderModule.PostRender();

		cecsar.Update<game::TransformSystem>();
	}

	SDL_Quit();
	return 0;
}
