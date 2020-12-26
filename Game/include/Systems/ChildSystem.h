#pragma once
#include "Components/ChildComponent.h"
#include "Components/Transform.h"
#include "JobSystem.h"

namespace game
{
	class ChildSystem final : public JobSystem<ChildComponent, Transform>
	{
	public:
		inline void UpdateManually(const int32_t& index);

	private:
		utils::SparseSet<ChildComponent>* _children = nullptr;
		utils::SparseSet<Transform>* _transforms = nullptr;
		std::vector<int32_t> _removables{};

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnUpdate(utils::SparseSet<ChildComponent>&, 
			utils::SparseSet<Transform>&) override;
	};

	inline void ChildSystem::UpdateManually(const int32_t& index)
	{
		auto& child = _children->Get(index);
		auto& transform = _transforms->Get(index);

		// Check if the parent is still there.
		if (!_transforms->Contains(child.parent))
		{
			_removables.push_back(index);
			return;
		}

		// Calculate world position based on child position.
		auto& localTransform = child.localTransform;
		auto position = localTransform.position;
		auto rotation = localTransform.rotation;

		auto& parent = _transforms->Get(child.parent);
		position = parent.position + position.Rotate(parent.rotation);
		rotation += parent.rotation;

		transform.position = position;
		transform.rotation = utils::Mathf::ConstrainAngle(rotation);
	}
}
