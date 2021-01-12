#include "Systems/LegSystem.h"
#include "Systems/TransformSystem.h"
#include "Modules/TimeModule.h"

void game::LegSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_transformSystem = &cecsar.GetSystem<TransformSystem>();
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::LegSystem::OnUpdate(
	utils::SparseSet<Leg>& legs, 
	utils::SparseSet<Joint>& joints, 
	utils::SparseSet<Transform>& transforms)
{
	const float deltaTime = _timeModule->GetDeltaTime();
	std::vector<int32_t> removables{};

	const auto dense = legs.GetDenseRaw();
	for (int32_t i = legs.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];
		auto& leg = legs[i];

		// Check if the root is valid.
		if(!_cecsar->IsEntityValid(leg.root))
		{
			removables.push_back(index);
			continue;
		}

		auto& joint = joints.Get(index);
		auto& transform = transforms.Get(index);

		auto& rootTransform = transforms.Get(leg.root.index);

		// Translate to world.
		const auto world = _transformSystem->ToWorld(transform);
		const auto rootWorld = _transformSystem->ToWorld(rootTransform, joint.offset);

		// Calculate offset and distance.
		const auto offset = world.position - rootWorld.position;
		const float dis = offset.Magnitude2d();

		if (leg.moving) 
		{
			// If it's too far away, break the movement.
			if (dis > leg.breakDistance) 
			{
				leg.moving = false;
				continue;
			}

			// If it's close enough.
			if(dis <= leg.stoppingDistance)
			{
				leg.moving = false;
				continue;
			}

			const auto dir = offset.Normalized2d();
			const auto delta = dir * std::min(leg.speed * deltaTime, dis);

			// Move towards target.
			const auto localDelta = _transformSystem->ToLocal(transform, delta);
			transform.position -= localDelta.position;
			continue;
		}

		// If it's not moving.
		if (dis > leg.moveThreshold) 
		{
			if(_cecsar->IsEntityValid(leg.mirror))
			{
				auto& mirror = legs.Get(leg.mirror.index);
				if (mirror.moving)
					continue;
			}

			leg.moving = true;
		}
	}

	// Remove unfit.
	for (auto removable : removables)
		legs.RemoveAt(removable);
}
