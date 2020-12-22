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

	jobSystem.Wait();

	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Spawn level.
	const game::LevelGenerator generator;
	generator.Generate(cecsar);

	// Spawn player for testing purposes.
	const auto player = cecsar.AddEntity<game::PlayerFactory>( 1);
	auto& set = cecsar.GetSet<game::Transform>();

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

		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::HandSystem>();
		cecsar.Update<game::LegSystem>();
		cecsar.Update<game::MovementSystem>();
		cecsar.Update<game::TransformSystem>();

		renderModule.PreRender();

		cecsar.Update<game::CameraSystem>();
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();

		jobSystem.Wait();
	}

	SDL_Quit();
	return 0;
}
