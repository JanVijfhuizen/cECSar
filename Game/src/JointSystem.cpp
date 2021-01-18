﻿#include "Systems/JointSystem.h"
#include "Systems/TransformSystem.h"
#include "Modules/TimeModule.h"

void game::JointSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<Joint, Transform>::Initialize(cecsar);

	_cecsar = &cecsar;
	_transformSystem = &cecsar.GetSystem<TransformSystem>();
	_timeModule = &cecsar.GetModule<TimeModule>();
	_deltas.reserve(cecsar.info.setCapacity);
}

void game::JointSystem::OnUpdate(
	utils::SparseSet<Joint>& joints, 
	utils::SparseSet<Transform>& transforms)
{
	std::mutex mut{};
	std::vector<int32_t> removables{};

	const float deltaTime = _timeModule->GetDeltaTime();
	const auto dense = joints.GetDenseRaw();

	auto& jobModule = GetJobConvModule();
	jobModule.ToLinearJobs(joints.GetCount(), 
		[this, &joints, &transforms, &removables, dense, deltaTime, &mut](
			const int32_t start, const int32_t stop) 
		{
			for (int32_t i = start; i < stop; ++i)
			{
				const int32_t index = dense[i];

				auto& joint = joints[i];
				auto& transform = transforms.Get(index);

				// Other validation check.
				if (!_cecsar->IsEntityValid(joint.other))
				{
					std::unique_lock<std::mutex> lk(mut);
					removables.push_back(index);
					continue;
				}

				// Get other information.
				const int32_t otherIndex = joint.other.index;
				auto& otherTransform = transforms.Get(otherIndex);

				// Translate to world information.
				const auto world = _transformSystem->ToWorld(transform);
				const auto rotatedOffset = joint.offset.Rotate(otherTransform.rotation);
				const auto otherWorld = _transformSystem->ToWorld(otherTransform, rotatedOffset);

				// Check offset and whether or not the parts are too far from eachother.
				const auto offset = world.position - otherWorld.position;
				const float dis = offset.Magnitude2d();
				const float intersection = dis - joint.maxDistance;

				// If it's close enough, there's nothing to be done.
				if (intersection < 0)
					continue;

				const auto offsetNormalized = offset.Normalized2d();

				// Move both objects to eachother until it's right at the maximum distance.
				const auto dir = _transformSystem->ToLocal(
					transform, offsetNormalized * intersection).position;

				JointDelta&& delta
				{
					index,
					dir * -(1.0f - joint.balance),
				};

				delta.rotation = transform.rotation - utils::Vector3::RotateTowards2d(
					transform.rotation, offsetNormalized, deltaTime * rotationSpeed);

				std::unique_lock<std::mutex> lk(mut);

				// Push delta information.
				_deltas.push_back(delta);

				// Only push the other if it's close.
				if (dis < joint.teleportDistance)
				{
					JointDelta&& otherDelta
					{
						otherIndex,
						dir * joint.balance,
					};

					_deltas.push_back(otherDelta);
				}
			}
		});

	jobModule.Start();
	jobModule.Wait();

	// Update positions all at once.
	for (auto& delta : _deltas)
	{
		auto& transform = transforms.Get(delta.index);
		transform.position += delta.value;
		transform.rotation += delta.rotation;
	}
	_deltas.clear();

	// Remove all unfit components.
	for (auto removable : removables)
		joints.RemoveAt(removable);
}