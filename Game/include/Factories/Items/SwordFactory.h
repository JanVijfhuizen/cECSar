#pragma once
#include "Factories/StandardFactory.h"

namespace game
{
	class SwordFactory : public StandardFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
