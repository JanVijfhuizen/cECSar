﻿#include <Systems/RigidBodySystem.h>
#include <Systems/TransformSystem.h>
#include "Modules/TimeModule.h"

void game::RigidBodySystem::Initialize(cecsar::Cecsar& cecsar)
{
	_rigidBodies = &cecsar.GetSet<RigidBody>();
	_timeModule = &cecsar.GetModule<TimeModule>();
	_transformSystem = &cecsar.GetSystem<TransformSystem>();

	cecsar.GetSystem<CollisionSystem>().Subscribe(*this);
}

void game::RigidBodySystem::OnUpdate(
	utils::SparseSet<RigidBody>& rigidBodies, 
	utils::SparseSet<Transform>& transforms)
{
	const float deltaTime = _timeModule->GetDeltaTime();

	const auto dense = rigidBodies.GetDenseRaw();
	for (int32_t i = rigidBodies.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];

		auto& rigidBody = rigidBodies[i];
		auto& transform = transforms.Get(index);

		// Add force as a vector to the transform position.
		const auto localForce = _transformSystem->ToLocal(transform, rigidBody.force);
		transform.position += localForce.position / rigidBody.weight;

		// Decrease force based on friction.
		const float delta = rigidBody.friction * deltaTime;
		const auto friction = utils::Vector3::One() * delta;
		rigidBody.force = rigidBody.force.Reduce(friction);

		// Add immediate force.
		transform.position += rigidBody.immediateForce;
		rigidBody.immediateForce = 0;
	}
}

void game::RigidBodySystem::OnNotify(const HitInfo& info)
{
	auto& instance = info.instance;

	auto& collider = instance.collider;
	auto& otherCollider = info.other.collider;

	if (collider->isStatic)
		return;

	// No adjusting needs to be done when they're both colliders.
	if (collider->isTrigger || otherCollider->isTrigger)
		return;
	
	const int32_t index = instance.index;
	if (!_rigidBodies->Contains(index))
		return;

	auto& rigidBody = _rigidBodies->Get(index);

	auto dir = info.intersection;
	if (!otherCollider->isStatic)
		dir /= 2;

	rigidBody.immediateForce -= dir;
}
