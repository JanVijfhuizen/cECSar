#pragma once
#include "Utils/Vector3.h"

namespace game
{
	class TransformSystem;

	struct Transform final
	{
		friend TransformSystem;

		utils::Vector3 position;
		float rotation = 0;

	private:
		int32_t _parent = -1;

		utils::Vector3 positionWorld;
		float rotationWorld = 0;
	};
}
