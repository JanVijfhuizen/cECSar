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
#include "GunFactory.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory
	{
	protected:
		cecsar::Cecsar* _cecsar = nullptr;
		RenderModule* _renderModule = nullptr;
		utils::SparseSet<LegComponent>* _legs = nullptr;
		utils::SparseSet<HandComponent>* _hands = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;
		utils::SparseSet<Renderer>* _renderers = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;
		inline void SpawnBodyParts(int32_t index) const;
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

	inline void PlayerFactory::OnConstruction(cecsar::Cecsar& cecsar, const int32_t index)
	{
		cecsar.AddComponent<Transform>(index);
		cecsar.AddComponent<Renderer>(index).texture = 
			_renderModule->GetTexture("Art/Player.png");
		cecsar.AddComponent<Controller>(index).type = ControllerType::player;
		cecsar.AddComponent<MovementComponent>(index);
		cecsar.AddComponent<CameraFollowTarget>(index);

		SpawnBodyParts(index);
	}

	inline void PlayerFactory::SpawnBodyParts(const int32_t index) const
	{
		// Feet.
		const auto feet = _cecsar->AddEntity<LegFactory>(2);
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& leg = _legs->Get(feet[i]);
			leg.parent = index;

			leg.offset.y = 16;
			leg.offset.x = 24 * (i * 2 - 1);
			leg.offset.z = -.05f;
			leg.other = feet[1 - i];
		}

		// Gun. For testing purposes.
		const auto gun = _cecsar->AddEntity<GunFactory>();
		TransformHelper::SetParent(*_transforms, gun[0], index);

		// Hands.
		const auto hands = _cecsar->AddEntity<HandFactory>(2);
		for (int32_t i = 0; i < 2; ++i)
		{
			auto& hand = _hands->Get(hands[i]);

			const auto flip = i ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			_renderers->Get(hands[i]).flip = flip;

			TransformHelper::SetParent(*_transforms, hands[i], index);

			hand.offset.y = 32;
			hand.offset.x = 24 * (i * 2 - 1);
			hand.offset.z = .05f;

			hand.target = gun[0];
		}
	}
}
