#pragma once
#include <variant>

namespace game
{
	constexpr uint32_t MASK_NONE = 0;
	constexpr uint32_t MASK_DEFAULT = 0b1;

	struct Collider final
	{
		struct Circle final
		{
			float radius = 16;
		};

		struct Rectangle final
		{
			float width = 16;
			float height = 16;
		};

		using Type = std::variant<Circle, Rectangle>;

		bool isTrigger = false;
		bool isStatic = false;

		uint32_t mask = MASK_DEFAULT;
		uint32_t targetMask = MASK_DEFAULT;

		Type type = Circle();
	};
}
