#pragma once
#include "ComponentSystem.h"
#include "Components/Transform.h"
#include "Modules/JobConverterModule.h"

namespace game
{
	class TransformSystem final : public cecsar::ComponentSystem<Transform>
	{
	public:
		~TransformSystem();

	private:
		cecsar::Cecsar* _cecsar = nullptr;
		JobConverterModule* _jobConverter = nullptr;

		int32_t* _sortableIndexes = nullptr;
		utils::SparseSet<Transform>* _transformBuffer = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
	};
}
