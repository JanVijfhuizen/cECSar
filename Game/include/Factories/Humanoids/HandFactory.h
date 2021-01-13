#pragma once
#include "LimbFactory.h"

namespace game
{
	class HandFactory : public LimbFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
