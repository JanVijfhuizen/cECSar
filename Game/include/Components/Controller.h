#pragma once

namespace game
{
	enum class ControllerType
	{
		ai,
		player
	};

	struct Controller final
	{
		ControllerType type = ControllerType::ai;
		int32_t xDir = 0, yDir = 0;
		int32_t xMouse = 0, yMouse = 0;
		bool rMouse = false, lMouse = false;
		bool space = false;
	};
}