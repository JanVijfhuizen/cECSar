#pragma once
#include <variant>
#include <Components/Transform.h>
#include <Utils/Quad.h>

namespace game
{
	constexpr uint32_t MASK_NONE = 0;
	constexpr uint32_t MASK_DEFAULT = 0b1;

	struct Collider final
	{
		struct Circle final
		{
			float radius;
		};

		struct Rectangle final
		{
			float width;
			float height;
		};

		using Type = std::variant<Circle, Rectangle>;

		bool isTrigger = false;
		bool isStatic = false;

		uint32_t mask = MASK_DEFAULT;
		uint32_t targetMask = MASK_DEFAULT;

		Type type = Circle();

		struct Visitor final
		{
			Transform& _world;
			const utils::Quad& _quad;

			Visitor(Transform& world, const utils::Quad& quad) :
				_world(world), _quad(quad)
			{
				
			}

			constexpr bool operator()(Circle& circle) const
			{
				const auto& position = _world.position;

				const float xCol = position.x;
				const float yCol = position.y;

				const float xQuad = _quad.pos.x;
				const float yQuad = _quad.pos.y;

				const float quadWidth = _quad.width;
				const float quadHeight = _quad.height;

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

			constexpr bool operator()(Rectangle& circle) const
			{
				return false;
			}
		};

		template <typename ...Args>
		static bool IntersectsQuad(Type& t, Args&& ... args)
		{
			return std::visit(Visitor{args...}, t);
		}
	};
}
