#pragma once
#include "EntityFactory.h"
#include "Components/HandComponent.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"

namespace game
{
	class HandFactory final : public cecsar::EntityFactory
	{
	protected:
		RenderModule* _renderModule = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;
	};

	inline void HandFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void HandFactory::OnConstruction(cecsar::Cecsar& cecsar, const int32_t index)
	{
		cecsar.AddComponent<HandComponent>(index);
		cecsar.AddComponent<Transform>(index);

		auto& renderer = cecsar.AddComponent<Renderer>(index);
		renderer.texture = _renderModule->GetTexture("Art/Oni.png");
		renderer.count = 6;
		renderer.index = 2;
	}
}
