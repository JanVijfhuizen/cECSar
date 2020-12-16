#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "Modules/RenderModule.h"
#include "BodyFactory.h"
#include "Components/BodyComponent.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory<Transform, Renderer,
		Controller, MovementComponent, CameraFollowTarget>
	{
	protected:
		cecsar::Cecsar* _cecsar = nullptr;
		RenderModule* _renderModule = nullptr;
		utils::SparseSet<BodyComponent>* _bodies = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&, Controller&, 
			MovementComponent&, CameraFollowTarget&) override;
	};

	inline void PlayerFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_cecsar = &cecsar;
		_renderModule = &cecsar.GetModule<RenderModule>();
		_bodies = &cecsar.GetSet<BodyComponent>();
	}

	inline void PlayerFactory::OnConstruction(const int32_t index, 
		Transform& transform, Renderer& renderer, Controller& controller, 
		MovementComponent&, CameraFollowTarget&)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Player.png");
		renderer.texture = texture;
		controller.type = ControllerType::player;

		const auto bodyParts = _cecsar->AddEntity<BodyFactory>(2);

		// Feet.
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& body = _bodies->Get(bodyParts[i]);
			body.parent = index;

			body.offset.y = -8;
			body.offset.x = 24 * (i * 2 - 1);
			body.other = bodyParts[1 - i];
		}

		delete[] bodyParts;
	}
}
