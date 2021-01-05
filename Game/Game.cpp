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
#include "Systems/KinematicSystem.h"
#include "Systems/CollisionSystem.h"

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

	// Systems.
	auto& collisionSystem = cecsar.GetSystem<game::CollisionSystem>();

	SDL_Event event;
	bool quit = false;

	//cecsar.AddEntity<game::EnvironmentFactory>();

	//cecsar.AddEntity<game::OniFactory>();
	for (int i = 0; i < 5; ++i)
		cecsar.AddEntity<game::RoninFactory>();

#pragma endregion

	while(!quit)
	{
		timeModule.Update();

		// Check for inputs.
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}

#pragma region Pre Buffers

#pragma endregion

		renderModule.PreRender();

		cecsar.Update<game::CameraSystem>();
		cecsar.Update<game::RenderSystem>();
		collisionSystem.Draw();

		renderModule.PostRender();

#pragma region Post Buffers
		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::MovementSystem>();
		cecsar.Update<game::KinematicSystem>();
#pragma endregion

#pragma region Cleanup
		cecsar.Update<game::TransformSystem>();
		cecsar.Update<game::CollisionSystem>();
#pragma endregion 

#pragma region Observer Calls
		collisionSystem.NotifyCollisions();
#pragma endregion
	}

	SDL_Quit();
	return 0;
}
