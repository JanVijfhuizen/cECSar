#pragma once
#include "EntityFactory.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Modules/RenderModule.h"

namespace game
{
	class BodyFactory final : public cecsar::EntityFactory<Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&) override;
	};

	inline void BodyFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void BodyFactory::OnConstruction(const int32_t index, Transform&, Renderer& renderer)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Feet.png");
		renderer.texture = texture;
	}
}
