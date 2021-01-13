#pragma once
#include "Factories/StandardFactory.h"
#include "Utils/Vector3.h"

namespace game
{
	class LimbFactory : public StandardFactory
	{
	public:
		utils::Vector3 offset{};

	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
