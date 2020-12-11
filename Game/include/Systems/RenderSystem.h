#pragma once
#include "ComponentSystem.h"
#include <Components/Renderer.h>
#include <Components/Transform.h>

namespace game
{
	class RenderSystem final : public cecsar::ComponentSystem<Renderer, Transform>
	{
	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) override;	
	};
}
