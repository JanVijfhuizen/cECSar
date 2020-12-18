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
#include <iostream>

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// THIS IS JUST SOME NONSENSE TESTING STUFF.
	// The interesting stuff happens in the ComponentSystems.

	// Setup cecsar.
	cecsar::CecsarInfo info;
	info.setCapacity = 2000;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Spawn level.
	const game::LevelGenerator generator;
	generator.Generate(cecsar);

	// Spawn player for testing purposes.
	const auto player = cecsar.AddEntity<game::PlayerFactory>(1);
	auto& set = cecsar.GetSet<game::Transform>();
	auto& transform = set.Get(player[0]);
	transform.posLocal.x = 100;
	transform.posLocal.y = 100;

	SDL_Event event;
	while(true)
	{
		set.Get(99).posLocal.x = sin(timeModule.GetTime()) * 256;
		set.Get(99).posLocal.y = cos(timeModule.GetTime()) * 256;

		timeModule.Update();

		while (SDL_PollEvent(&event) != 0)
			;

		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::HandSystem>();
		cecsar.Update<game::LegSystem>();
		cecsar.Update<game::MovementSystem>();
		cecsar.Update<game::TransformSystem>();

		renderModule.PreRender();

		cecsar.Update<game::CameraSystem>();
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();
	}

	SDL_Quit();
	return 0;
}
