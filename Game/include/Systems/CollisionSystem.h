#pragma once
#include "JobSystem.h"
#include "Components/Transform.h"
#include "Components/Collider.h"

namespace game
{
	class CollisionSystem final : JobSystem<Collider, Transform>
	{
	public:

	};
}
