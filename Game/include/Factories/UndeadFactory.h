#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"

namespace game
{
	class UndeadFactory final : public cecsar::EntityFactory<Transform, Renderer>
	{
	protected:
		RenderModule* _renderModule = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&) override;
	};

	inline void UndeadFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_renderModule = &cecsar.GetModule<RenderModule>();
	}

	inline void UndeadFactory::OnConstruction(const int32_t index,
		Transform& transform, Renderer& renderer)
	{
		renderer.texture = _renderModule->GetTexture("Art/Undead.png");
		renderer.count = 3;
	}
}
