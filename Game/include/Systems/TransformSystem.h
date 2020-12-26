#pragma once
#include "Components/Transform.h"
#include "JobSystem.h"

namespace game
{
	class TransformSystem final : public JobSystem<Transform>
	{
	public:
		~TransformSystem();

		static constexpr int32_t GetParent(const Transform& t);
		constexpr void SetParent(const int32_t& child, const int32_t& parent) const;

		static inline utils::Vector3 ToWorld(const Transform& t, const utils::Vector3& local);
		static inline utils::Vector3 ToLocal(const Transform& t, const utils::Vector3& global);

		static inline utils::Vector3 GetWorldPosition(const Transform& t);
		static inline float GetWorldRotation(const Transform& t);

	private:
		int32_t* _sortableIndexes = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
	};

	constexpr int32_t TransformSystem::GetParent(const Transform& t)
	{
		return t._parent;
	}

	constexpr void TransformSystem::SetParent(
		const int32_t& child, const int32_t& parent) const
	{
		auto& childTransform = _transforms->Get(child);
		childTransform._parent = parent;
	}

	inline utils::Vector3 TransformSystem::ToWorld(
		const Transform& t, const utils::Vector3& local)
	{
		return t.positionWorld - (t.position - local).Rotate(t.rotationWorld);
	}

	inline utils::Vector3 TransformSystem::ToLocal(
		const Transform& t, const utils::Vector3& global)
	{
		return (global - t.positionWorld).Rotate(-t.rotationWorld);
	}

	inline utils::Vector3 TransformSystem::GetWorldPosition(const Transform& t)
	{
		return t.positionWorld;
	}

	inline float TransformSystem::GetWorldRotation(const Transform& t)
	{
		return t.rotationWorld;
	}
}
