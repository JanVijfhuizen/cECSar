#pragma once
#include <Factories/StandardFactory.h>
#include <Utils/Vector3.h>

namespace game
{
	class HeadFactory : public StandardFactory
	{
	public:
		utils::Vector3 offset{ 0, 8, .01f };

	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};
}
