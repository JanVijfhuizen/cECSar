#pragma once
#include "Info/CollisionInfo.h"
#include "Components/Collider.h"

namespace game
{
#pragma region Intershapes Checks
	inline bool CircleRectangleIntersects(
		Collider::Circle& a, Collider::Rectangle& b, CollisionInfo& info);
#pragma endregion 

#pragma region Visitors
	struct CircleCollisionVisitor final
	{
		Collider::Circle& a;
		CollisionInfo& info;

		inline CircleCollisionVisitor(
			Collider::Circle& a, CollisionInfo& info);

		inline bool operator()(Collider::Circle& b) const;
		inline bool operator()(Collider::Rectangle& b) const;
	};

	struct RectangleCollisionVisitor final
	{
		Collider::Rectangle& a;
		CollisionInfo& info;

		inline RectangleCollisionVisitor(Collider::Rectangle& a, CollisionInfo& info);

		inline bool operator()(Collider::Circle& b) const;
		inline bool operator()(Collider::Rectangle& b) const;
	};

	struct ShapeCollisionVisitor final
	{
		Collider::Type& a;
		CollisionInfo info;

		inline ShapeCollisionVisitor(Collider::Type& a, CollisionInfo&& info);

		inline bool operator()(Collider::Circle& b);
		inline bool operator()(Collider::Rectangle& b);
	};
#pragma endregion 

	inline bool CircleRectangleIntersects(
		Collider::Circle& a, Collider::Rectangle& b,
		CollisionInfo& info)
	{
		// Do a normal check.
		return false;
	}

	inline CircleCollisionVisitor::CircleCollisionVisitor(
		Collider::Circle& a, CollisionInfo& info) :
		a(a), info(info)
	{

	}

	inline bool CircleCollisionVisitor::operator()(Collider::Circle& b) const
	{
		const float threshold = a.radius / 2 + b.radius / 2;
		const auto dir = info.aWorld.position - info.bWorld.position;
		const float dirMagnitude = dir.Magnitude();

		// Check if in range.
		if (dirMagnitude > threshold)
			return false;

		const float intersection = (dirMagnitude - threshold) / 2;
		info.aIntersectOut = dir * intersection;
		info.bIntersectOut = dir * -intersection;

		info.aPointOut = dir * (a.radius / 2) + info.aIntersectOut;
		info.bPointOut = dir * (-b.radius / 2) + info.bIntersectOut;
		return true;
	}

	inline bool CircleCollisionVisitor::operator()(Collider::Rectangle& b) const
	{
		return CircleRectangleIntersects(a, b, info);
	}

	inline RectangleCollisionVisitor::RectangleCollisionVisitor(
		Collider::Rectangle& a, CollisionInfo& info) :
		a(a), info(info)
	{

	}

	inline bool RectangleCollisionVisitor::operator()(Collider::Circle& b) const
	{
		auto reverse = info.Reverse();
		return CircleRectangleIntersects(b, a, reverse);
	}

	inline bool RectangleCollisionVisitor::operator()(Collider::Rectangle& b) const
	{
		// Do a normal check.
		return false;
	}

	inline ShapeCollisionVisitor::ShapeCollisionVisitor(
		Collider::Type& a, CollisionInfo&& info) :
		a(a), info(info)
	{

	}

	inline bool ShapeCollisionVisitor::operator()(Collider::Circle& b)
	{
		auto reverse = info.Reverse();
		return std::visit(
			CircleCollisionVisitor{b, reverse}, a);
	}

	inline bool ShapeCollisionVisitor::operator()(Collider::Rectangle& b)
	{
		auto reverse = info.Reverse();
		return std::visit(
			RectangleCollisionVisitor{b, reverse}, a);
	}
}
