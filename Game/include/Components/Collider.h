#pragma once

namespace game
{
	constexpr uint32_t MASK_NONE = 0;
	constexpr uint32_t MASK_DEFAULT = 0b1;

	enum class ColliderType
	{
		Circle,
		Rectangle
	};

	struct Collider final
	{
		ColliderType type = ColliderType::Circle;

		bool isTrigger = false;
		bool isStatic = false;

		uint32_t mask = MASK_DEFAULT;
		uint32_t targetMask = MASK_DEFAULT;

		union
		{
			struct Circle
			{
				float radius;
			} circle;

			struct Rectangle
			{
				float width, height;
			} rectangle{};
		};

		inline Collider(ColliderType type = ColliderType::Circle);
	};

	inline Collider::Collider(const ColliderType type) : type(type)
	{
		switch (type)
		{
		case ColliderType::Circle:
			circle.radius = 16;
			break;
		case ColliderType::Rectangle:
			rectangle.width = 16;
			rectangle.height = 16;
			break;
		default:
			return;
		}
	}
}
