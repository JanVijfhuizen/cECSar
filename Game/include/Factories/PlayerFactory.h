#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"

namespace game
{
	class PlayerFactory final : public cecsar::EntityFactory<Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&) override;	
	};

	inline void PlayerFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void PlayerFactory::OnConstruction(const int32_t index, 
		Transform& transform, Renderer& renderer)
	{
		transform.x = rand() % 800;
		transform.y = rand() % 600;
		transform.z = .01f * (rand() % 100);

		renderer.texture = _renderModule->GetTexture("Art/Wall.png");
		renderer.count = 1;
	}
}
