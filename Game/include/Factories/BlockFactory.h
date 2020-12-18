#pragma once
#include "EntityFactory.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class BlockFactory final : public cecsar::EntityFactory
	{
	protected:
		RenderModule* _renderModule = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;
	};

	inline void BlockFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void BlockFactory::OnConstruction(cecsar::Cecsar& cecsar, const int32_t index)
	{
		cecsar.AddComponent<Renderer>(index).texture = _renderModule->GetTexture("Art/Block.png");
		cecsar.AddComponent<Transform>(index);
	}
}
