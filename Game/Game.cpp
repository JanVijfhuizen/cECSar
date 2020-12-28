#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/ControllerSystem.h"
#include "Modules/TimeModule.h"
#include "Systems/CameraSystem.h"
#include "Modules/JobSystemModule.h"
#include "Factories/Humanoids/OniFactory.h"
#include "Factories/Humanoids/RoninFactory.h"
#include "Factories/Environment/EnvironmentFactory.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// THIS IS JUST SOME NONSENSE TESTING STUFF.
	// The interesting stuff happens in the ComponentSystems.

	// Setup cecsar.
	cecsar::CecsarSettings info;
	info.setCapacity = 2e3;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	SDL_Event event;
	bool quit = false;

	cecsar.AddEntity<game::EnvironmentFactory>();

	const auto oni = cecsar.AddEntity<game::OniFactory>()[0];
	const auto ronin = cecsar.AddEntity<game::RoninFactory>()[0];

	cecsar.GetSet<game::Transform>().Get(oni.index).position = { 200, 200, .1f };
	cecsar.GetSet<game::Transform>().Get(ronin.index).position = { 100, 100, .1f };

	cecsar.GetSet<game::Transform>().Get(ronin.index).parent = oni.index;

#pragma endregion

	while(!quit)
	{
		cecsar.GetSet<game::Transform>().Get(oni.index).rotation = 
			sin(timeModule.GetTime()) * 180;

		timeModule.Update();

		// Check for inputs.
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}

		renderModule.PreRender();

		cecsar.Update<game::CameraSystem>();
		cecsar.Update<game::RenderSystem>();

		renderModule.PostRender();

#pragma region Post Buffers
		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::MovementSystem>();
#pragma endregion
	}

	SDL_Quit();
	return 0;
}
