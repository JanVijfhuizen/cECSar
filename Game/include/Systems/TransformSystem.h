#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"

namespace game
{
	class TransformSystem final : public cecsar::ComponentSystem<Transform>
	{
	private:
		cecsar::Cecsar* _cecsar = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
		void ClearHangingObjects(utils::SparseSet<Transform>& transforms);
		void UpdateWorldTransforms(utils::SparseSet<Transform>& transforms);

		static float SortDepth(const Transform& transform, int32_t index);	
	};
}
