#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Components/Controller.h"
#include "Components/MovementComponent.h"
#include "Modules/RenderModule.h"
#include "LegFactory.h"
#include "Components/LegComponent.h"
#include "Components/HandComponent.h"
#include "HandFactory.h"
#include "Helpers/TransformHelper.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory<Transform, Renderer,
		Controller, MovementComponent, CameraFollowTarget>
	{
	protected:
		cecsar::Cecsar* _cecsar = nullptr;
		RenderModule* _renderModule = nullptr;
		utils::SparseSet<LegComponent>* _legs = nullptr;
		utils::SparseSet<HandComponent>* _hands = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;
		utils::SparseSet<Renderer>* _renderers = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&, Controller&, 
			MovementComponent&, CameraFollowTarget&) override;
	};

	inline void PlayerFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_cecsar = &cecsar;
		_renderModule = &cecsar.GetModule<RenderModule>();

		_legs = &cecsar.GetSet<LegComponent>();
		_hands = &cecsar.GetSet<HandComponent>();
		_transforms = &cecsar.GetSet<Transform>();
		_renderers = &cecsar.GetSet<Renderer>();
	}

	inline void PlayerFactory::OnConstruction(const int32_t index, 
		Transform&, Renderer& renderer, Controller& controller, 
		MovementComponent&, CameraFollowTarget&)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Player.png");
		renderer.texture = texture;
		controller.type = ControllerType::player;

		// Feet.
		const auto feet = _cecsar->AddEntity<LegFactory>(2);
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& leg = _legs->Get(feet[i]);
			leg.parent = index;

			leg.offset.y = 16;
			leg.offset.x = 24 * (i * 2 - 1);
			leg.other = feet[1 - i];
		}

		delete[] feet;

		// Gun. For testing purposes.
		const auto gun = _cecsar->AddEntity();
		auto& gunTransform = _cecsar->AddComponent<Transform>(gun[0]);
		gunTransform.posLocal.z = .3f;
		gunTransform.posLocal.y = 40;
		gunTransform.posLocal.x = 20;

		auto& gunRenderer = _cecsar->AddComponent<Renderer>(gun[0]);
		gunRenderer.texture = _renderModule->GetTexture("Art/Gun.png");
		
		TransformHelper::SetParent(*_transforms, gun[0], index);

		// Hands.
		const auto hands = _cecsar->AddEntity<HandFactory>(2);
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& hand = _hands->Get(hands[i]);

			const auto flip = i ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			_renderers->Get(hands[i]).flip = flip;

			TransformHelper::SetParent(*_transforms, hands[i], index);

			hand.offset.y = 34;
			hand.offset.x = 18 * (i * 2 - 1);

			hand.target = gun[0];
		}

		delete[] gun;
		delete[] hands;
	}
}
