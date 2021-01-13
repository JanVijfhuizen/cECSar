#pragma once
#include "LimbFactory.h"

namespace game
{
	class LegFactory : public LimbFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
