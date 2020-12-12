#pragma once
#include "EntityFactory.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class BlockFactory final : public cecsar::EntityFactory<Renderer, Transform>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(int32_t index, Renderer&, Transform&) override;
	};

	inline void BlockFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void BlockFactory::OnConstruction(const int32_t index, 
		Renderer& renderer, Transform& transform)
	{
		renderer.texture = _renderModule->GetTexture("Art/Block.png");
	}
}
