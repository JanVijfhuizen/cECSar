#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/TransformSystem.h"
#include "Systems/RenderSystem.h"
#include "Factories/BlockFactory.h"
#include "Systems/MovementSystem.h"
#include "Systems/ControllerSystem.h"
#include "Modules/TimeModule.h"
#include "Systems/CameraSystem.h"
#include "Managers/LevelGenerator.h"
#include "Factories/PlayerFactory.h"
#include "Systems/LegSystem.h"
#include "Systems/HandSystem.h"
#include "Modules/JobSystemModule.h"
#include <iostream>
#include "Modules/BufferModule.h"

void Job(int32_t x, int32_t y)
{
	
}

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// THIS IS JUST SOME NONSENSE TESTING STUFF.
	// The interesting stuff happens in the ComponentSystems.

	// Setup cecsar.
	cecsar::CecsarInfo info;
	info.setCapacity = 5000;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& jobSystem = cecsar.GetModule<game::JobSystemModule>();
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	// Buffers.
	auto& transformBuffer = cecsar.GetModule<game::BufferModule<game::Transform>>();
	auto& renderBuffer = cecsar.GetModule<game::BufferModule<game::Renderer>>();
	auto& controllerBuffer = cecsar.GetModule<game::BufferModule<game::Controller>>();

	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Spawn level.
	const game::LevelGenerator generator;
	generator.Generate(cecsar);

	// Spawn player for testing purposes.
	const auto player = cecsar.AddEntity<game::PlayerFactory>( 700);
	auto& set = cecsar.GetSet<game::Transform>();

	for (int i = 0; i < 700; ++i)
	{
		set.Get(player[i]).posLocal = { float(rand() % 20), float(rand() % 20) };
	}

	SDL_Event event;
	bool quit = false;

	while(!quit)
	{
		set.Get(99).posLocal.x = sin(timeModule.GetTime()) * 256;
		set.Get(99).posLocal.y = cos(timeModule.GetTime()) * 256;

		timeModule.Update();

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}

		// Update Buffers.
		transformBuffer.UpdateBuffer();
		renderBuffer.UpdateBuffer();
		controllerBuffer.UpdateBuffer();

#pragma region Independently Threadable
		// Sets Controllers.
		cecsar.Update<game::ControllerSystem>();

		// Sets Transforms.
		cecsar.Update<game::MovementSystem>();

		jobSystem.Wait();
#pragma endregion 

		// These all get and set transforms, so they can't be threaded at the same time.
		cecsar.Update<game::HandSystem>();
		cecsar.Update<game::LegSystem>();
		cecsar.Update<game::TransformSystem>();

		// Update rendersystems.
		renderModule.PreRender();

		cecsar.Update<game::CameraSystem>();
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();

		// Wait for threads to join.
		jobSystem.Wait();
	}

	SDL_Quit();
	return 0;
}
