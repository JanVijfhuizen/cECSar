#pragma once
#include "Components/Transform.h"
#include "JobSystem.h"

namespace game
{
	class TransformSystem final : public JobSystem<Transform>
	{
	public:
		~TransformSystem();

	private:
		int32_t* _sortableIndexes = nullptr;
		utils::SparseSet<Transform>* _transformBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
	};
}
