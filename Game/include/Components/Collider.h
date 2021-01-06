#pragma once
#include <variant>
#include <Components/Transform.h>
#include <Utils/Onion.h>
#include <Utils/Quad.h>

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

	inline bool IntersectsQuad(Collider::Circle& circle, 
		Transform& world, utils::Quad& quad)
	{
		const auto& position = world.position;

		const float xCol = position.x;
		const float yCol = position.y;

		const float xQuad = quad.pos.x;
		const float yQuad = quad.pos.y;

		const float quadWidth = quad.width;
		const float quadHeight = quad.height;

		const float radiusHalf = circle.radius / 2;

		// Horizontal check.
		if (xCol - radiusHalf < xQuad ||
			yCol - radiusHalf < yQuad)
			return false;

		// Vertical check.
		if (xCol + radiusHalf >= xQuad + quadWidth ||
			yCol + radiusHalf >= yQuad + quadHeight)
			return false;
			
		return true;
	}

	VARIANT_FN(IntersectsQuad, bool);
}
