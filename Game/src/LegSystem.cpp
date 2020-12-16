#include "Systems/LegSystem.h"
#include "Modules/TimeModule.h"

void game::LegSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::LegSystem::OnUpdate(
	utils::SparseSet<LegComponent>& legs, utils::SparseSet<MovementComponent>& movements, 
	utils::SparseSet<Transform>& transforms)
{
	const float deltaTime = _timeModule->GetDeltaTime();

	const auto iterator = legs.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& body = legs[i];
		if (body.parent == -1)
			continue;

		auto& transform = transforms.Get(iterator[i]);
		auto& parentTransform = transforms.Get(body.parent);
		const auto& parentMovement = movements.Get(body.parent);

		// Calculate target position.
		const auto rotatedTargetOffset = body.offset.Rotate(parentTransform.rotGlobal);
		auto targetPosition = parentTransform.posGlobal + rotatedTargetOffset;
		const utils::Vector3 offset = targetPosition - transform.posGlobal;

		// Calculate offset.
		const float magnitude = offset.Magnitude2d();

		// Teleport if too far away.
		if (magnitude > body.teleportThreshold)
		{
			transform.posLocal = parentTransform.posGlobal;
			continue;
		}

		// Move if it's too far away and the linked bodypart isn't moving either.
		if(!body.moving)
			if (magnitude > body.moveThreshold)
			{
				if (body.other == -1)
					body.moving = true;
				else if(!legs.Get(body.other).moving)
					body.moving = true;
			}

		// Somehow it doesnt center.
		if (body.moving)
		{
			// If the bodypart is close enough.
			if (magnitude < body.stoppingDistance)
			{
				body.moving = false;
				continue;
			}

			const auto&& offsetNormalized = offset.Normalized2d();
			const float&& delta = parentMovement.movementSpeed * body.speedMultiplier * deltaTime;

			transform.posLocal += offsetNormalized * delta;
			transform.rot = parentTransform.rot;
		}

		transform.posLocal.z = parentTransform.posGlobal.z - .05f;
	}
}
