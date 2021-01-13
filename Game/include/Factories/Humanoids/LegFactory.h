#pragma once
#include "Factories/StandardFactory.h"
#include "Utils/Vector3.h"

namespace game
{
	class LegFactory : public StandardFactory
	{
	public:
		utils::Vector3 offset{ 16, 8, -.01f };

	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
