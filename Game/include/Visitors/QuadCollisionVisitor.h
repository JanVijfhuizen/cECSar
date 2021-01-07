#pragma once
#include "Components/Transform.h"
#include "Utils/Quad.h"
#include "Components/Collider.h"

namespace game
{
	struct QuadCollisionVisitor final
	{
		const Transform& world;
		const utils::Quad& quad;

		constexpr QuadCollisionVisitor(const Transform& world, const utils::Quad& quad);

		constexpr bool operator()(Collider::Circle& circle) const;
		constexpr bool operator()(Collider::Rectangle& circle) const;
	};

	constexpr bool QuadCollisionVisitor::operator()(
		Collider::Circle& circle) const
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

	constexpr bool QuadCollisionVisitor::operator()(
		Collider::Rectangle& circle) const
	{
		return false;
	}

	constexpr QuadCollisionVisitor::QuadCollisionVisitor(
		const Transform& world, const utils::Quad& quad):
		world(world), quad(quad)
	{

	}
}
