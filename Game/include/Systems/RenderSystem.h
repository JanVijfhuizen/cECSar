#pragma once
#include "ComponentSystem.h"
#include <Components/Renderer.h>
#include <Components/Transform.h>

namespace game
{
	class RenderModule;

	class RenderSystem final : public cecsar::ComponentSystem<Renderer, Transform>
	{
	private:
		RenderModule* _module = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) override;

		static float Sort(const Renderer& renderer, int32_t index);
	};
}
