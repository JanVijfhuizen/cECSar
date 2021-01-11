#pragma once
#include <cstdint>
#include "Utils/Vector3.h"

namespace game
{
	struct KeyFrame final
	{
		int32_t index = 0;
		float max = 1;

		utils::Vector3 startOffset{};
		utils::Vector3 stopOffset{};
	};

	struct Animator final
	{
		bool paused = false;
		bool repeat = true;

		float speed = 1;
		float lerp = 0;

		std::vector<KeyFrame> keyFrames{};
		int32_t keyframeOffset = 0;
	};
}
