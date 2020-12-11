#pragma once
#include "ComponentSystem.h"
#include <Components/Renderer.h>
#include <Components/Transform.h>
#include "Modules/RenderModule.h"

namespace game
{
	class RenderSystem final : public cecsar::ComponentSystem<Renderer, Transform>
	{
	private:
		RenderModule* _module = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) override;
	};
}
