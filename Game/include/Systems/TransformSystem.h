#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"

namespace game
{
	class TransformSystem final : public cecsar::ComponentSystem<Transform>
	{
	public:
		~TransformSystem();

	private:
		cecsar::Cecsar* _cecsar = nullptr;
		int32_t* _sortableIndexes = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
		void ClearHangingObjects(utils::SparseSet<Transform>& transforms) const;
		void UpdateGlobalPositions(utils::SparseSet<Transform>& transforms);
	};
}
