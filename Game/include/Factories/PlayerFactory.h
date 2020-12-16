#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "Modules/RenderModule.h"
#include "LegFactory.h"
#include "Components/LegComponent.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory<Transform, Renderer,
		Controller, MovementComponent, CameraFollowTarget>
	{
	protected:
		cecsar::Cecsar* _cecsar = nullptr;
		RenderModule* _renderModule = nullptr;
		utils::SparseSet<LegComponent>* _legs = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&, Controller&, 
			MovementComponent&, CameraFollowTarget&) override;
	};

	inline void PlayerFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_cecsar = &cecsar;
		_renderModule = &cecsar.GetModule<RenderModule>();
		_legs = &cecsar.GetSet<LegComponent>();
	}

	inline void PlayerFactory::OnConstruction(const int32_t index, 
		Transform&, Renderer& renderer, Controller& controller, 
		MovementComponent&, CameraFollowTarget&)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Player.png");
		renderer.texture = texture;
		controller.type = ControllerType::player;

		// Feet.
		const auto feet = _cecsar->AddEntity<BodyFactory>(2);
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& body = _legs->Get(feet[i]);
			body.parent = index;

			body.offset.y = 16;
			body.offset.x = 24 * (i * 2 - 1);
			body.other = feet[1 - i];
		}

		delete[] feet;
	}
}
