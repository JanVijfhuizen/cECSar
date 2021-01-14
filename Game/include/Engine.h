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
#include <Systems/AnimatorSystem.h>
#include <Systems/JointSystem.h>

// Factories.
#include <Factories/Humanoids/RoninFactory.h>
#include <Factories/Environment/EnvironmentFactory.h>
#include <Factories\Humanoids\OniFactory.h>
#include "Systems/LegSystem.h"

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

		/*
		Setup references, initialize modules, etcetera.
		*/
		static inline void Initialize(Info& out);
		/*
		Start of the actual game, spawn environment, player etc.
		*/
		static inline void Start(Info& info);
		/*
		Start threads, update buffers etc.
		*/
		static inline void PreUpdate(Info& info);
		/*
		Single threaded update call, can use the jobsystem however.
		*/
		static inline void Update(Info& info);
		/*
		Single threaded render update, be aware of dependencies.
		*/
		static inline void RenderUpdate(Info& info);
		/*
		Occasional call to the fixed update, which happens N times per second,
		instead of being bound to the framerate.
		*/
		static inline void FixedUpdate(Info& info);
		/*
		Cleanup, observer calls, etc.
		*/
		static inline void PostUpdate(Info& info);
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

			PreUpdate(info);

			// TODO: thread separate.
			RenderUpdate(info);

			Update(info);

			// TODO: thread separate.
			// Calculate physics update.
			for (int32_t i = info.timeModule->GetPhysicsSteps() - 1; i >= 0; --i)
				FixedUpdate(info);

			PostUpdate(info);
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
		//info.cecsar->AddEntity<EnvironmentFactory>();
		const auto oni = info.cecsar->AddEntity<OniFactory>()[0];
		const auto ronin = info.cecsar->AddEntity<RoninFactory>()[0];

		info.cecsar->GetSet<Transform>().Get(oni.index).position = { 240, 120 };
	}

	inline void Engine::PreUpdate(Info& info)
	{
	}

	inline void Engine::Update(Info& info)
	{
		auto& cecsar = *info.cecsar;

		cecsar.Update<CameraSystem>();
		cecsar.Update<ControllerSystem>();

		cecsar.Update<MovementSystem>();
		cecsar.Update<KinematicSystem>();
		cecsar.Update<LegSystem>();

		cecsar.Update<JointSystem>();
		cecsar.Update<TransformSystem>();
	}

	inline void Engine::RenderUpdate(Info& info)
	{
		auto& cecsar = *info.cecsar;

		info.renderModule->PreRender();

		cecsar.Update<RenderSystem>();
		//cecsar.GetSystem<CollisionSystem>().DrawDebug();

		info.renderModule->PostRender();
	}

	inline void Engine::FixedUpdate(Info& info)
	{
		auto& cecsar = *info.cecsar;
		cecsar.Update<CollisionSystem>();
	}

	inline void Engine::PostUpdate(Info& info)
	{
		auto& cecsar = *info.cecsar;
		info.collisionSystem->NotifyCollisions();

		// Dependent on notifications.
		cecsar.Update<RigidBodySystem>();

		// Dependent on renderers.
		cecsar.Update<AnimatorSystem>();
	}
}
