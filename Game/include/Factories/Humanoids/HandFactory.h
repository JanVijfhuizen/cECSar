#pragma once
#include <Factories/StandardFactory.h>
#include "Utils/Vector3.h"

namespace game
{
	class HandFactory : public StandardFactory
	{
	public:
		utils::Vector3 offset{16, 16, -.01f};

	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
