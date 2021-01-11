#pragma once
#include <cstdint>
#include <variant>

namespace game
{
	struct KeyFrame final
	{
		int32_t index = 0;
		float max = 1;
	};

	struct Animator final
	{
		struct Simple final
		{
			int32_t start = 0, stop = -1;
		};

		struct Advanced final
		{
			std::vector<KeyFrame> keyFrames{};
		};

		using Type = std::variant<Simple, Advanced>;

		Type type = Simple();

		bool paused = false;
		bool repeat = true;

		float speed = 4;
		float lerp = 0;

		int32_t keyframeOffset = 0;
	};
}
