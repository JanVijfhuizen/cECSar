#pragma once
#include "Components/Transform.h"
#include "Utils/Quad.h"
#include "Components/Collider.h"

namespace game
{
	struct QuadCollisionVisitor final
	{
	public:
		constexpr QuadCollisionVisitor(const Transform& world, const utils::Quad& quad);

		constexpr bool operator()(Collider::Circle& circle) const;
		constexpr bool operator()(Collider::Rectangle& circle) const;

	private:
		const Transform& _world;
		const utils::Quad& _quad;
	};

	constexpr bool QuadCollisionVisitor::operator()(
		Collider::Circle& circle) const
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

	constexpr bool QuadCollisionVisitor::operator()(
		Collider::Rectangle& circle) const
	{
		return false;
	}

	constexpr QuadCollisionVisitor::QuadCollisionVisitor(
		const Transform& world, const utils::Quad& quad):
		_world(world), _quad(quad)
	{

	}
}
