#pragma once
#include <Factories/StandardFactory.h>

namespace game
{
	class HandFactory : public StandardFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
