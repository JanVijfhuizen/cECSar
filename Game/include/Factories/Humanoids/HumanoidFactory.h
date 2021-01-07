#pragma once
#include <Factories/StandardFactory.h>

namespace game
{
	class HumanoidFactory : public StandardFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
