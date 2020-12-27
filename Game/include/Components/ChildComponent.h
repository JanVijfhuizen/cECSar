#pragma once
#include "Transform.h"

namespace game
{
	class ChildSystem;

	struct ChildComponent final
	{
		friend ChildSystem;

		bool updateManually = false;
		int32_t parent = -1;
		Transform localTransform{};

	private:
		int32_t _parentId = -1;
	};
}
