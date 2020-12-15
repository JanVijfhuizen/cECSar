#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "Modules/RenderModule.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory<Transform, Renderer,
		Controller, MovementComponent, CameraFollowTarget>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&, Controller&, 
			MovementComponent&, CameraFollowTarget&) override;
	};

	inline void PlayerFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void PlayerFactory::OnConstruction(const int32_t index, 
		Transform&, Renderer& renderer, Controller& controller, 
		MovementComponent&, CameraFollowTarget&)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Player.png");
		renderer.texture = texture;
		controller.type = ControllerType::player;
	}
}
