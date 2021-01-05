#pragma once

namespace game
{
	constexpr uint32_t MASK_NONE = 0;
	constexpr uint32_t MASK_DEFAULT = 0b1;

	struct Collider final
	{
		float radius = 16;
		bool isTrigger;

		uint32_t mask = MASK_DEFAULT;
		uint32_t targetMask = MASK_DEFAULT;
	};
}
