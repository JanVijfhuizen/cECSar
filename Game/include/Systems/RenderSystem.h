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
		struct RendererSorter
		{
			RendererSorter(utils::SparseSet<Transform>& set);
			bool operator() (int32_t a, int32_t b) const;

		private:
			utils::SparseSet<Transform>& _set;
		};

		RenderModule* _module = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) override;
	};
}
