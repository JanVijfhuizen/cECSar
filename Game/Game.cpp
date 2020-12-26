#include "Cecsar.h"
#include "SDL.h"
#include "Modules/RenderModule.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/ControllerSystem.h"
#include "Modules/TimeModule.h"
#include "Systems/CameraSystem.h"
#include "Modules/JobSystemModule.h"
#include "Modules/BufferModule.h"
#include "Systems/ChildSystem.h"
#include "Factories/OniFactory.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	// THIS IS JUST SOME NONSENSE TESTING STUFF.
	// The interesting stuff happens in the ComponentSystems.

	// Setup cecsar.
	cecsar::CecsarSettings info;
	info.setCapacity = 5e3;
	cecsar::Cecsar cecsar{ info };

	// Modules.
	auto& timeModule = cecsar.GetModule<game::TimeModule>();
	auto& renderModule = cecsar.GetModule<game::RenderModule>();

	// Buffers.
	auto& transformBuffer = cecsar.GetModule<game::BufferModule<game::Transform>>();
	auto& renderBuffer = cecsar.GetModule<game::BufferModule<game::Renderer>>();
	auto& controllerBuffer = cecsar.GetModule<game::BufferModule<game::Controller>>();

	SDL_Event event;
	bool quit = false;

	cecsar.AddEntity<game::OniFactory>();

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

		// Check for inputs.
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}

#pragma region Pre Buffers
		cecsar.Update<game::ChildSystem>();
#pragma endregion

#pragma region Updating Buffers
		transformBuffer.UpdateBuffer();
		renderBuffer.UpdateBuffer();
		controllerBuffer.UpdateBuffer();
#pragma endregion 

		// Notify the render thread.
		cv_renderer.notify_one();

#pragma region Post Buffers
		cecsar.Update<game::ControllerSystem>();
		cecsar.Update<game::MovementSystem>();
#pragma endregion 

		std::unique_lock<std::mutex> lock(mutexRenderer);
	}

	SDL_Quit();
	return 0;
}
