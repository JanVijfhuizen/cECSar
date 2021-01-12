#pragma once
#include "JobSystem.h"
#include "Components/Transform.h"

namespace game
{
	class TransformSystem final : public JobSystem<Transform>
	{
	public:
		[[nodiscard]] Transform ToWorld(
			const Transform& t, const utils::Vector3& p = {}) const;

		[[nodiscard]] Transform ToLocal(
			const Transform& t, const utils::Vector3& p = {}) const;

	private:
		cecsar::Cecsar* _cecsar = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>&) override;
	};
}
