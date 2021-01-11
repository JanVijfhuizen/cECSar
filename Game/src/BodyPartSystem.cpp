#include "Systems/BodyPartSystem.h"
#include "Systems/TransformSystem.h"

void game::BodyPartSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_transformSystem = &cecsar.GetSystem<TransformSystem>();
}

void game::BodyPartSystem::OnUpdate(
	utils::SparseSet<BodyPart>& bodyParts, 
	utils::SparseSet<Kinematic>& kinematics,
	utils::SparseSet<Transform>& transforms)
{
	std::vector<int32_t> removables{};

	const auto dense = bodyParts.GetDenseRaw();
	for (int32_t i = bodyParts.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];
		auto& bodyPart = bodyParts[i];

		// Validate root.
		auto& rootInfo = bodyPart.root;
		if(!_cecsar->IsEntityValid(rootInfo))
		{
			removables.push_back(rootInfo.index);
			continue;
		}

		auto& rootTransform = transforms.Get(rootInfo.index);
		const auto rootWorld = _transformSystem->ToWorld(rootTransform);

		// Calculate delta and update root position.
		const auto delta = rootWorld.position - bodyPart.rootPosition;
		bodyPart.rootPosition = rootWorld.position;
	}

	// Remove all parts without a root.
	for (int32_t index : removables)
		bodyParts.RemoveAt(index);
}
