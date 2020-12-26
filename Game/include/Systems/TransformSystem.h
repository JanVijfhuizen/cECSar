#pragma once
#include "Components/Transform.h"
#include "JobSystem.h"

namespace game
{
	class TransformSystem final : public JobSystem<Transform>
	{
	public:
		~TransformSystem();

		static inline int32_t GetParent(Transform& t);
		constexpr void SetParent(const int32_t& child, const int32_t& parent) const;

		static inline utils::Vector3 ToWorld(Transform& t, const utils::Vector3& local);
		static inline utils::Vector3 ToLocal(Transform& t, const utils::Vector3& global);

	private:
		int32_t* _sortableIndexes = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<Transform>& transforms) override;
	};

	int32_t TransformSystem::GetParent(Transform& t)
	{
		return t._parent;
	}

	constexpr void TransformSystem::SetParent(const int32_t& child, const int32_t& parent) const
	{
		auto& childTransform = _transforms->Get(child);
		childTransform._parent = parent;
	}

	utils::Vector3 TransformSystem::ToWorld(Transform& t, const utils::Vector3& local)
	{
		return t.posGlobal - (t.posLocal - local).Rotate(t.rotGlobal);
	}

	utils::Vector3 TransformSystem::ToLocal(Transform& t, const utils::Vector3& global)
	{
		return (global - t.posGlobal).Rotate(-t.rotGlobal);
	}
}
