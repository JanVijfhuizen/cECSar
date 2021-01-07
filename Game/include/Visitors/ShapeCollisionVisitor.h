#pragma once
#include <Components/Collider.h>
#include <Components/Transform.h>

namespace game
{
#pragma region Intershapes Checks
	inline bool CircleRectangleIntersects(
		Collider::Circle& circle, Collider::Rectangle& rectangle,
		const Transform& circleWorld, const Transform& rectangleWorld);
#pragma endregion 

#pragma region Visitors
	struct CircleCollisionVisitor
	{
	public:
		inline CircleCollisionVisitor(Collider::Circle& circle,
			const Transform& aWorld, const Transform& bWorld);

		inline bool operator()(Collider::Circle& circle) const;
		inline bool operator()(Collider::Rectangle& rectangle) const;

	private:
		Collider::Circle& _circle;
		const Transform& _aWorld, _bWorld;
	};

	struct RectangleCollisionVisitor
	{
	public:
		inline RectangleCollisionVisitor(Collider::Rectangle& rectangle,
			const Transform& aWorld, const Transform& bWorld);

		inline bool operator()(Collider::Circle& circle) const;
		inline bool operator()(Collider::Rectangle& rectangle) const;

	private:
		Collider::Rectangle& _rectangle;
		const Transform& _aWorld, _bWorld;
	};

	struct ShapeCollisionVisitor final
	{
	public:
		inline ShapeCollisionVisitor(Collider::Type& type,
			const Transform& aWorld, const Transform& bWorld);

		inline bool operator()(Collider::Circle& circle) const;

		inline bool operator()(Collider::Rectangle& rectangle) const;

	private:
		Collider::Type& _type;
		const Transform& _aWorld, _bWorld;
	};
#pragma endregion 

	inline bool CircleRectangleIntersects(
		Collider::Circle& circle, Collider::Rectangle& rectangle,
		const Transform& circleWorld, const Transform& rectangleWorld)
	{
		// Do a normal check.
		return false;
	}

	inline CircleCollisionVisitor::CircleCollisionVisitor(Collider::Circle& circle, 
		const Transform& aWorld, const Transform& bWorld) : 
		_circle(circle), _aWorld(aWorld), _bWorld(bWorld)
	{

	}

	inline bool CircleCollisionVisitor::operator()(Collider::Circle& circle) const
	{
		// Do a normal check.
		return false;
	}

	inline bool CircleCollisionVisitor::operator()(Collider::Rectangle& rectangle) const
	{
		return CircleRectangleIntersects(_circle, rectangle, 
			_aWorld, _bWorld);
	}

	inline RectangleCollisionVisitor::RectangleCollisionVisitor(Collider::Rectangle& rectangle, 
		const Transform& aWorld, const Transform& bWorld) :
		_rectangle(rectangle), _aWorld(aWorld), _bWorld(bWorld)
	{

	}

	inline bool RectangleCollisionVisitor::operator()(Collider::Circle& circle) const
	{
		return CircleRectangleIntersects(circle, _rectangle,
			_bWorld, _aWorld);
	}

	inline bool RectangleCollisionVisitor::operator()(Collider::Rectangle& rectangle) const
	{
		// Do a normal check.
		return false;
	}

	inline ShapeCollisionVisitor::ShapeCollisionVisitor(Collider::Type& type, 
		const Transform& aWorld, const Transform& bWorld) :
		_type(type), _aWorld(aWorld), _bWorld(bWorld)
	{

	}

	inline bool ShapeCollisionVisitor::operator()(Collider::Circle& circle) const
	{
		return std::visit(CircleCollisionVisitor{
			circle, _bWorld, _aWorld
		}, _type);
	}

	inline bool ShapeCollisionVisitor::operator()(Collider::Rectangle& rectangle) const
	{
		return std::visit(RectangleCollisionVisitor{
			rectangle, _bWorld, _aWorld
		}, _type);
	}
}
