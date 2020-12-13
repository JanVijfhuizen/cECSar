#pragma once
#include "Components/Transform.h"
#include "SparseSet.h"

namespace game
{
	struct TransformHelper final
	{
		static void SetParent(utils::SparseSet<Transform>& transforms, 
			int32_t child, int32_t parent);
	};

	inline void TransformHelper::SetParent(utils::SparseSet<Transform>& transforms, 
		const int32_t child, const int32_t parent)
	{
		auto& childTransform = transforms[child];

		// Decrease depth.
		int32_t index = childTransform.parent;
		while(index != -1)
		{
			Transform& transform = transforms[index];
			index = transform.parent;

			transform.rDepth--;
		}

		// Set parent.
		childTransform.parent = parent;

		// Increase depth.
		index = parent;
		while(index != -1)
		{
			Transform& transform = transforms[index];
			index = transform.parent;

			transform.rDepth++;
		}
	}
}
