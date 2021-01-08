#pragma once
#include <Factories/StandardFactory.h>
#include "Utils/Vector3.h"

namespace game
{
	class HandFactory : public StandardFactory
	{
	public:
		utils::Vector3 offset{20, 10, -.05f};

	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
