#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/TransformSystem.h"
#include "Systems/RenderSystem.h"
#include "Factories/BlockFactory.h"
#include "Helpers/TransformHelper.h"
#include <cassert>
#include "Factories/UndeadFactory.h"
#include "Systems/MovementSystem.h"
#include "Systems/ControllerSystem.h"
#include "Modules/TimeModule.h"
#include <iostream>

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// THIS IS JUST SOME NONSENSE TESTING STUFF.
	// The interesting stuff happens in the ComponentSystems.

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
	auto& renderers = cecsar.GetSet<game::Renderer>();

	// Setup some nonsense to experiment with.
	const int32_t WIDTH = 12, HEIGHT = 8;
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

			transform.posLocal.x = x * posMod + x * padding;
			transform.posLocal.y = y * posMod + y * padding;

			transform.posLocal.z = -.2f * (x + y);
			transform.rot = x + y;

			auto& renderer = renderers[*index];
			renderer.color.r = x * 10;
			renderer.color.g = y * 10;
			renderer.color.b = abs(transform.posLocal.z) * 10;
		}
	}
	delete [] ptrs;

	const int32_t* ptrsMoving = cecsar.AddEntity<game::UndeadFactory>(2);
	game::TransformHelper::SetParent(transforms, ptrsMoving[1], ptrsMoving[0]);
	//delete[] ptrsMoving;
	transforms.Get(ptrsMoving[0]).posLocal.y = 100;

	auto& timeModule = cecsar.GetModule<game::TimeModule>();

	const auto& player = cecsar.AddEntity<game::UndeadFactory>(1);
	cecsar.GetSet<game::Controller>().Get(player[0]).type = game::ControllerType::player;
	delete[] player;

	SDL_Event event;
	while(true)
	{
		timeModule.Update();

		while (SDL_PollEvent(&event) != 0)
			;

		const float time = timeModule.GetTime();

		transforms.Get(ptrsMoving[0]).rot += timeModule.GetDeltaTime() * 50;
		transforms.Get(ptrsMoving[0]).posLocal.x = 200 + sin(time) * 100;
		transforms.Get(ptrsMoving[0]).posLocal.z = sin(time) * 2;
		transforms.Get(ptrsMoving[1]).posLocal.y = cos(time) * 100;

		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::MovementSystem>();
		cecsar.Update<game::TransformSystem>();

		renderModule.PreRender();
		cecsar.Update<game::RenderSystem>();
		renderModule.PostRender();
	}

	SDL_Quit();
	return 0;
}
