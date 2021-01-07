#pragma once
#include "Info/CollisionInfo.h"
#include "Components/Collider.h"

namespace game
{
	inline bool CircleRectangleIntersects(
		const Collider::Circle& a, const Collider::Rectangle& b, CollisionInfo& info);

	struct CircleCollisionVisitor final
	{
		const Collider::Circle& a;
		CollisionInfo& info;

		inline CircleCollisionVisitor(
			const Collider::Circle& a, CollisionInfo& info);

		inline bool operator()(const Collider::Circle& b) const;
		inline bool operator()(const Collider::Rectangle& b) const;
	};

	struct RectangleCollisionVisitor final
	{
		const Collider::Rectangle& a;
		CollisionInfo& info;

		inline RectangleCollisionVisitor(
			const Collider::Rectangle& a, CollisionInfo& info);

		inline bool operator()(const Collider::Circle& b) const;
		inline bool operator()(const Collider::Rectangle& b) const;
	};

	struct ShapeCollisionVisitor final
	{
		const Collider::Type& a;
		CollisionInfo& info;

		inline ShapeCollisionVisitor(
			const Collider::Type& a, CollisionInfo& info);

		inline bool operator()(const Collider::Circle& b) const;
		inline bool operator()(const Collider::Rectangle& b) const;
	};

	inline bool CircleRectangleIntersects(
		const Collider::Circle& a, const Collider::Rectangle& b,
		CollisionInfo& info)
	{
		// Do a normal check.
		return false;
	}

	inline CircleCollisionVisitor::CircleCollisionVisitor(
		const Collider::Circle& a, CollisionInfo& info) :
		a(a), info(info)
	{

	}

	inline bool CircleCollisionVisitor::operator()(
		const Collider::Circle& b) const
	{
		const float threshold = a.radius / 2 + b.radius / 2;
		const auto dir = info.aWorld->position - info.bWorld->position;
		const float dirMagnitude = dir.Magnitude();

		// Check if in range.
		if (dirMagnitude > threshold)
			return false;

		const float intersection = (dirMagnitude - threshold) / 2;
		*info.aIntersectOut = dir * intersection;
		*info.bIntersectOut = dir * -intersection;

		*info.aPointOut = dir * (a.radius / 2) + *info.aIntersectOut;
		*info.bPointOut = dir * (-b.radius / 2) + *info.bIntersectOut;
		return true;
	}

	inline bool CircleCollisionVisitor::operator()(
		const Collider::Rectangle& b) const
	{
		return CircleRectangleIntersects(a, b, info);
	}

	inline RectangleCollisionVisitor::RectangleCollisionVisitor(
		const Collider::Rectangle& a, CollisionInfo& info) :
		a(a), info(info)
	{

	}

	inline bool RectangleCollisionVisitor::operator()(
		const Collider::Circle& b) const
	{
		auto reverse = info.Reverse();
		return CircleRectangleIntersects(b, a, reverse);
	}

	inline bool RectangleCollisionVisitor::operator()(
		const Collider::Rectangle& b) const
	{
		// Do a normal check.
		return false;
	}

	inline ShapeCollisionVisitor::ShapeCollisionVisitor(
		const Collider::Type& a, CollisionInfo& info) :
		a(a), info(info)
	{

	}

	inline bool ShapeCollisionVisitor::operator()(
		const Collider::Circle& b) const
	{
		auto reverse = info.Reverse();
		return std::visit(
			CircleCollisionVisitor{b, reverse}, a);
	}

	inline bool ShapeCollisionVisitor::operator()(
		const Collider::Rectangle& b) const
	{
		auto reverse = info.Reverse();
		return std::visit(
			RectangleCollisionVisitor{b, reverse}, a);
	}
}
