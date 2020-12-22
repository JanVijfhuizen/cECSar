#pragma once
#include "ComponentSystem.h"
#include <Components/Renderer.h>
#include "Components/Transform.h"
#include "Modules/BufferModule.h"

namespace game
{
	class RenderModule;

	class RenderSystem final : public cecsar::ComponentSystem<Renderer>
	{
	public:
		~RenderSystem();

	private:
		struct RenderInfo final
		{
			int32_t index = 0;
			float order = 0;

			RenderInfo();
			RenderInfo(int32_t index, float order);
		};

		RenderModule* _module = nullptr;
		RenderInfo* _sortableInfo = nullptr;
		utils::SparseSet<Transform>* _transformBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Renderer>&) override;
		void SortIndexes(utils::SparseSet<Renderer>&) const;
	};
}
