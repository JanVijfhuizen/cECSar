#include "Systems/JointSystem.h"
#include "Systems/TransformSystem.h"
#include "Modules/TimeModule.h"

void game::JointSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<Joint, RigidBody, Transform>::Initialize(cecsar);

	_cecsar = &cecsar;
	_transformSystem = &cecsar.GetSystem<TransformSystem>();
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::JointSystem::OnUpdate(
	utils::SparseSet<Joint>& joints,
	utils::SparseSet<RigidBody>& rigidBodies,
	utils::SparseSet<Transform>& transforms)
{
	std::mutex forceMutex{};
	std::mutex removablesMutex{};
	std::vector<int32_t> removables{};

	const float deltaTime = _timeModule->GetDeltaTime();
	auto* const dense = joints.GetDenseRaw();

	auto& jobModule = GetJobConvModule();
	jobModule.ToLinearJobs(joints.GetCount(), 
		[this, &joints, &rigidBodies, &transforms, &removables, 
			dense, deltaTime, &removablesMutex, &forceMutex](
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
					std::unique_lock<std::mutex> lk(removablesMutex);
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

				// Check offset and whether or not the parts are too far from each other.
				const auto offset = world.position - otherWorld.position;
				const float dis = offset.Magnitude2d();
				const float intersection = dis - joint.maxDistance;

				// If it's close enough, there's nothing to be done.
				if (intersection < 0)
					continue;

				const auto offsetNormalized = offset.Normalized2d();

				// Move both objects to each other until it's right at the maximum distance.
				const auto dir = _transformSystem->ToLocal(
					transform, offsetNormalized * intersection).position;

				auto& rigidBody = rigidBodies.Get(index);
				auto& otherRigidBody = rigidBodies.Get(otherIndex);

				// Calculate the balance between the two joints.
				const float balance = utils::Mathf::Clamp(.5f + rigidBody.weight - otherRigidBody.weight);

				std::unique_lock<std::mutex> lk(forceMutex);

				// Apply force.
				const auto force = dir * -(1 - balance);
				rigidBody.immediateForce += force;
				
				// Only push the other if it's close.
				if (dis < joint.teleportDistance)
				{
					const auto otherForce = dir * balance;
					otherRigidBody.immediateForce += otherForce;
				}
			}
		});

	jobModule.Start();
	jobModule.Wait();

	// Remove all unfit components.
	for (auto removable : removables)
		joints.RemoveAt(removable);
}
