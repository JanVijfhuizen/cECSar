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
	info.setCapacity = 5e3;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& jobSystem = cecsar.GetModule<game::JobSystemModule>();
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	// Buffers.
	auto& transformBuffer = cecsar.GetModule<game::BufferModule<game::Transform>>();
	auto& renderBuffer = cecsar.GetModule<game::BufferModule<game::Renderer>>();
	auto& controllerBuffer = cecsar.GetModule<game::BufferModule<game::Controller>>();

#pragma region Testing Nonsense
	renderModule.zMod = .1;
	renderModule.zColorFallof = .2f;

	// Spawn level.
	const game::LevelGenerator generator;
	generator.Generate(cecsar);

	// Spawn player for testing purposes.
	const auto player = cecsar.AddEntity<game::PlayerFactory>();
	auto& set = cecsar.GetSet<game::Transform>();

	SDL_Event event;
	bool quit = false;

	const auto others = cecsar.AddEntity<game::PlayerFactory>(800);
	for (int i = 0; i < 800; ++i)
	{
		set.Get(others[i]).posLocal = 
		{ float(rand() % 600) - 300, float(rand() % 600 - 300) };
	}
#pragma endregion

#pragma region Render Thread
	std::mutex mutexRenderer{};
	std::condition_variable cv_renderer{};

	std::thread renderThread([&mutexRenderer, &cv_renderer, &cecsar, &renderModule]()
		{
			while (true)
			{
				std::unique_lock<std::mutex> lock(mutexRenderer);
				cv_renderer.wait(lock);

				renderModule.PreRender();

				cecsar.Update<game::CameraSystem>();
				cecsar.Update<game::RenderSystem>();

				renderModule.PostRender();
			}
		});
#pragma endregion 

	while(!quit)
	{
		timeModule.Update();

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}

#pragma region Updating Buffers
		transformBuffer.UpdateBuffer();
		renderBuffer.UpdateBuffer();
		controllerBuffer.UpdateBuffer();
#pragma endregion 

		cv_renderer.notify_one();

#pragma region Independent Threading
		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::TransformSystem>();

		jobSystem.Wait();
#pragma endregion 

#pragma region Sequenced Threading
		cecsar.Update<game::MovementSystem>();
		jobSystem.Wait();

		cecsar.Update<game::HandSystem>();
		jobSystem.Wait();

		cecsar.Update<game::LegSystem>();
		jobSystem.Wait();
#pragma endregion

		std::unique_lock<std::mutex> lock(mutexRenderer);
	}

	SDL_Quit();
	return 0;
}
