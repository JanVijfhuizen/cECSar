#pragma once
#include "EntityFactory.h"
#include "Components/LegComponent.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class LegFactory final : public cecsar::EntityFactory
	{
	protected:
		RenderModule* _renderModule = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;
	};

	inline void LegFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void LegFactory::OnConstruction(cecsar::Cecsar& cecsar, const int32_t index)
	{
		cecsar.AddComponent<LegComponent>(index);
		cecsar.AddComponent<Transform>(index);
		cecsar.AddComponent<Renderer>(index).texture = _renderModule->GetTexture("Art/Leg.png");
	}
}
