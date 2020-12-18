#pragma once
#include "Components/Transform.h"
#include "SparseSet.h"

namespace game
{
	struct TransformHelper final
	{
		static constexpr void SetParent(utils::SparseSet<Transform>& transforms, 
			int32_t child, int32_t parent);

		static inline utils::Vector3 ToWorld(Transform& t, const utils::Vector3& local);
		static inline utils::Vector3 ToLocal(Transform& t, const utils::Vector3& global);
	};

	constexpr void TransformHelper::SetParent(utils::SparseSet<Transform>& transforms,
		const int32_t child, const int32_t parent)
	{
		auto& childTransform = transforms.Get(child);
		const int32_t rDepth = childTransform.rDepth + 1;

		// Decrease depth.
		int32_t index = childTransform.parent;
		while(index != -1)
		{
			Transform& transform = transforms.Get(index);
			index = transform.parent;

			transform.rDepth -= rDepth;
		}

		// Set parent.
		childTransform.parent = parent;

		// Increase depth.
		index = parent;
		while(index != -1)
		{
			Transform& transform = transforms.Get(index);
			index = transform.parent;

			transform.rDepth += rDepth;
		}
	}

	inline utils::Vector3 TransformHelper::ToWorld(Transform& t, const utils::Vector3& local)
	{
		return t.posGlobal - (t.posLocal - local).Rotate(t.rotGlobal);
	}

	inline utils::Vector3 TransformHelper::ToLocal(Transform& t, const utils::Vector3& global)
	{
		return (global - t.posGlobal).Rotate(t.rotGlobal);
	}
}
