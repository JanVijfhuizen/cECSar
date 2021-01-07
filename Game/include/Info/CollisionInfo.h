#pragma once
#include "Utils/Vector3.h"
#include "Components/Transform.h"

namespace game
{
	struct CollisionInfo
	{
		const Transform* const aWorld;
		const Transform* const bWorld;

		utils::Vector3* const aPointOut;
		utils::Vector3* const bPointOut;

		utils::Vector3* const aIntersectOut;
		utils::Vector3* const bIntersectOut;

		inline CollisionInfo(
			const Transform* aWorld, const Transform* bWorld,
			utils::Vector3* aPointOut, utils::Vector3* bPointOut,
			utils::Vector3* aIntersectOut, utils::Vector3* bIntersectOut);

		[[nodiscard]] inline CollisionInfo Reverse() const;
	};

	inline CollisionInfo::CollisionInfo(
		const Transform* aWorld, const Transform* bWorld,
		utils::Vector3* aPointOut, utils::Vector3* bPointOut, 
		utils::Vector3* aIntersectOut, utils::Vector3* bIntersectOut) :

		aWorld(aWorld), bWorld(bWorld), aPointOut(aPointOut), bPointOut(bPointOut),
		aIntersectOut(aIntersectOut), bIntersectOut(bIntersectOut)
	{
	}

	inline CollisionInfo CollisionInfo::Reverse() const
	{
		return CollisionInfo(bWorld, aWorld, bPointOut,
			aPointOut, bIntersectOut, aIntersectOut);
	}
}
