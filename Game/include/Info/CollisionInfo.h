#pragma once
#include "Utils/Vector3.h"
#include "Components/Transform.h"

namespace game
{
	struct CollisionInfo
	{
		const Transform& aWorld, bWorld;
		utils::Vector3& aPointOut, bPointOut;
		utils::Vector3& aIntersectOut, bIntersectOut;

		inline CollisionInfo(
			const Transform& aWorld, const Transform& bWorld,
			utils::Vector3& aPointOut, utils::Vector3& bPointOut,
			utils::Vector3& aIntersectOut, utils::Vector3& bIntersectOut);

		constexpr CollisionInfo Reverse();
	};

	inline CollisionInfo::CollisionInfo(
		const Transform& aWorld, const Transform& bWorld,
		utils::Vector3& aPointOut, utils::Vector3& bPointOut,
		utils::Vector3& aIntersectOut, utils::Vector3& bIntersectOut) :

		aWorld(aWorld), bWorld(bWorld), aPointOut(aPointOut), bPointOut(bPointOut),
		aIntersectOut(aIntersectOut), bIntersectOut(bIntersectOut)
	{

	}

	constexpr CollisionInfo CollisionInfo::Reverse()
	{
		return { bWorld, aWorld, bPointOut,
			aPointOut, bIntersectOut, aIntersectOut };
	}
}
