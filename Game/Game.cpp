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
#include "Systems/CameraSystem.h"
#include "Managers/LevelGenerator.h"

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
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Spawn level.
	const game::LevelGenerator generator;
	delete [] generator.Generate(cecsar);

	// Spawn player for testing purposes.
	const auto& player = cecsar.AddEntity<game::UndeadFactory>(1);
	cecsar.GetSet<game::Controller>().Get(player[0]).type = game::ControllerType::player;
	cecsar.AddComponent<game::CameraFollowTarget>(player[0]);
	delete[] player;

	SDL_Event event;
	while(true)
	{
		timeModule.Update();

		while (SDL_PollEvent(&event) != 0)
			;

		cecsar.Update<game::ControllerSystem>();
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
