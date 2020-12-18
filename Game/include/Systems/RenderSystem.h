#pragma once
#include "ComponentSystem.h"
#include <Components/Renderer.h>
#include <Components/Transform.h>

namespace game
{
	class RenderModule;

	class RenderSystem final : public cecsar::ComponentSystem<Renderer, Transform>
	{
	public:
		~RenderSystem();

	private:
		RenderModule* _module = nullptr;
		int32_t* _sortableIndexes = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) override;
		void SortIndexes(utils::SparseSet<Renderer>&, utils::SparseSet<Transform>&) const;
	};
}
