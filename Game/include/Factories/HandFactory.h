#pragma once
#include "EntityFactory.h"
#include "Components/HandComponent.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"

namespace game
{
	class HandFactory final : public cecsar::EntityFactory<HandComponent, Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(int32_t index, HandComponent&, Transform&, Renderer&) override;
	};

	inline void HandFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void HandFactory::OnConstruction(const int32_t index, HandComponent&, Transform&, Renderer& renderer)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Hand.png");
		renderer.texture = texture;
	}
}
