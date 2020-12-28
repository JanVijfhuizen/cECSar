#include <Systems/KinematicSystem.h>
#include "Systems/TransformSystem.h"

void game::KinematicSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_transformSystem = &cecsar.GetSystem<TransformSystem>();
}

void game::KinematicSystem::OnUpdate(
	utils::SparseSet<Kinematic>& kinematics, utils::SparseSet<Transform>& transforms)
{
	const int32_t count = kinematics.GetCount();
	const auto dense = kinematics.GetDenseRaw();

	std::vector<int32_t> removables;

	for (int32_t i = count - 1; i >= 0; --i)
	{
		auto& kinematic = kinematics[i];
		auto& transform = transforms.Get(dense[i]);

		// Check if target is valid.
		if (!_cecsar->IsEntityValid(kinematic.target))
			continue;

		// Check if parent is invalid.
		if(!_cecsar->IsEntityValid(transform.parent))
		{
			removables.push_back(dense[i]);
			continue;
		}

		// Interpolate between root and target.	
		auto& parent = transforms.Get(transform.parent.index);
		const auto root = _transformSystem->ToWorld(parent, kinematic.offset);
		auto& targetTransform = transforms.Get(kinematic.target.index);
		const auto target = _transformSystem->ToWorld(targetTransform);

		// Set position.
		const auto point = root.position.MoveTowards2dClamped(
			target.position, kinematic.max);
		auto pointLocal = _transformSystem->ToLocal(transform, point);

		// Clamp z position.
		pointLocal.position.z = kinematic.offset.z;
		transform.position = pointLocal.position;
	}

	// Remove all kinematics without a target or parent.
	for (int32_t index : removables)
		kinematics.RemoveAt(index);
}
