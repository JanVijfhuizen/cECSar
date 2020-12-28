#pragma once
#include "IModule.h"
#include "Utils/Vector3.h"
#include "Components/Transform.h"

namespace game
{
	class TransformModule final : public cecsar::IModule
	{
	public:
		static inline utils::Vector3 ToWorldSpace(
			const Transform& t, const utils::Vector3& v);
		static inline utils::Vector3 ToLocalSpace(
			const Transform& t, const utils::Vector3& v);

		static inline void SetToWorldSpace(
			const Transform& parent, Transform& child);
		static inline void SetToLocalSpace(
			const Transform& parent, Transform& child);
	};

	inline utils::Vector3 TransformModule::ToWorldSpace(
		const Transform& t, const utils::Vector3& v)
	{
		return t.position + v.Rotate(t.rotation);
	}

	inline utils::Vector3 TransformModule::ToLocalSpace(
		const Transform& t, const utils::Vector3& v)
	{
		return v.Rotate(-t.rotation) - t.position;
	}

	inline void TransformModule::SetToWorldSpace(const Transform& parent, Transform& child)
	{
		child.position = ToWorldSpace(parent, child.position);
		child.rotation -= parent.rotation;
	}

	inline void TransformModule::SetToLocalSpace(const Transform& parent, Transform& child)
	{
		child.position = ToLocalSpace(parent, child.position);
		child.rotation += parent.rotation;
	}
}
