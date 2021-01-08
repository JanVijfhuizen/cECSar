#pragma once
#include <SDL.h>
#include <Cecsar.h>

// Modules.
#include <Modules/TimeModule.h>
#include <Modules/RenderModule.h>

// Systems.
#include <Systems/CollisionSystem.h>
#include <Systems/RenderSystem.h>
#include <Systems/CameraSystem.h>
#include <Systems/MovementSystem.h>
#include <Systems/KinematicSystem.h>
#include <Systems/ControllerSystem.h>
#include <Systems/RigidBodySystem.h>

// Factories.
#include <Factories/Humanoids/RoninFactory.h>
#include <Factories/Environment/EnvironmentFactory.h>
#include <Factories\Humanoids\OniFactory.h>

namespace game
{
	class Engine final
	{
	public:
		static inline void Run();

	private:
		struct Info
		{
			bool quit = false;
			cecsar::Cecsar* cecsar = nullptr;

			// Modules.
			TimeModule* timeModule = nullptr;
			RenderModule* renderModule = nullptr;

			// Systems.
			CollisionSystem* collisionSystem = nullptr;
		};

		static inline void Initialize(Info& out);
		static inline void Start(Info& info);
		static inline void Update(Info& info);
	};

	inline void Engine::Run()
	{
		SDL_Init(0);

		Info info{};

		Initialize(info);
		Start(info);

		SDL_Event event;
		while (!info.quit)
		{
			info.timeModule->Update();

			// Check for inputs.
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
					info.quit = true;
			}

			Update(info);
		}

		SDL_Quit();
	}

	inline void Engine::Initialize(Info& out)
	{
		cecsar::CecsarSettings settings;
		settings.setCapacity = 1e4;

		auto cecsar = out.cecsar = new cecsar::Cecsar(settings);

		out.timeModule = &cecsar->GetModule<TimeModule>();
		out.renderModule = &cecsar->GetModule<RenderModule>();

		out.collisionSystem = &cecsar->GetSystem<CollisionSystem>();
	}

	inline void Engine::Start(Info& info)
	{
		info.cecsar->AddEntity<EnvironmentFactory>();
		info.cecsar->AddEntity<OniFactory>();
		info.cecsar->AddEntity<RoninFactory>();
	}

	inline void Engine::Update(Info& info)
	{
		auto& cecsar = *info.cecsar;

#pragma region Pre Buffers

#pragma endregion

		info.renderModule->PreRender();

		cecsar.Update<CameraSystem>();
		cecsar.Update<RenderSystem>();
		//collisionSystem.DrawDebug();

		info.renderModule->PostRender();

#pragma region Post Buffers
		cecsar.Update<ControllerSystem>();
		cecsar.Update<MovementSystem>();
		cecsar.Update<KinematicSystem>();
#pragma endregion

#pragma region Cleanup
		cecsar.Update<TransformSystem>();
		cecsar.Update<CollisionSystem>();
#pragma endregion 

#pragma region Observer Calls
		info.collisionSystem->NotifyCollisions();
#pragma endregion

#pragma region Observers
		cecsar.Update<RigidBodySystem>();
#pragma endregion 
	}
}
