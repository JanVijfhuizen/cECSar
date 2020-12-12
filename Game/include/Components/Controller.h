#pragma once

namespace game
{
	enum class ControllerType
	{
		none,
		player,
		ai
	};

	struct Controller final
	{
		ControllerType type;
	};
}