#pragma once
#include "Components/Transform.h"
#include "Components/HandComponent.h"
#include "JobSystem.h"

namespace game
{
	class HandSystem final : public JobSystem<HandComponent, Transform>
	{
	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;

	private:
		void OnUpdate(utils::SparseSet<HandComponent>&, utils::SparseSet<Transform>&) override;
	};
}
