#pragma once
#include "EntityFactory.h"
#include "Modules/RenderModule.h"
#include "Components/Renderer.h"
#include "Components/CameraFollowTarget.h"

namespace game
{
	class GunFactory final : public cecsar::EntityFactory
	{
	protected:
		RenderModule* _renderModule = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;
	};

	inline void GunFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void GunFactory::OnConstruction(cecsar::Cecsar& cecsar, const int32_t index)
	{
		cecsar.AddComponent<Transform>(index);
		auto& renderer = cecsar.AddComponent<Renderer>(index);
		renderer.texture = _renderModule->GetTexture("Art/Oni.png");
		renderer.count = 6;
		renderer.index = 5;

		cecsar.AddComponent<CameraFollowTarget>(index);
	}
}