#pragma once
#include "EntityFactory.h"
#include "Components/LegComponent.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class LegFactory final : public cecsar::EntityFactory<LegComponent, Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, LegComponent&, Transform&, Renderer&) override;
	};

	inline void LegFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void LegFactory::OnConstruction(const int32_t index, LegComponent&, Transform&, Renderer& renderer)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Leg.png");
		renderer.texture = texture;
	}
}
