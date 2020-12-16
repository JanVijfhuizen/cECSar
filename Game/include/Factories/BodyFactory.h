#pragma once
#include "EntityFactory.h"
#include "Components/BodyComponent.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class BodyFactory final : public cecsar::EntityFactory<BodyComponent, Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, BodyComponent&, Transform&, Renderer&) override;
	};

	inline void BodyFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void BodyFactory::OnConstruction(int32_t index, BodyComponent&, Transform&, Renderer& renderer)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Leg.png");
		renderer.texture = texture;
		renderer.color.c4 = _mm_set_ps1(220);
	}
}
