#include "Systems/BodySystem.h"
#include "Modules/TimeModule.h"
#include "Factories/BodyFactory.h"

void game::BodySystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::BodySystem::OnUpdate(
	utils::SparseSet<BodyComponent>& bodies, utils::SparseSet<MovementComponent>& movements, 
	utils::SparseSet<Transform>& transforms)
{
	const float deltaTime = _timeModule->GetDeltaTime();

	const auto iterator = bodies.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& body = bodies[i];
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

		// Move if it's too far away and the linked bodypart isn't moving either.
		if(!body.moving)
			if (magnitude > body.moveThreshold)
				if (body.other == -1)
					body.moving = true;
				else
				{
					if (!bodies.Get(body.other).moving)
						body.moving = true;
				}

		// Somehow it doesnt center.
		if (body.moving)
		{
			// TODO Teleport if too far away.

			// If the bodypart is close enough.
			if (magnitude < body.stoppingDistance)
			{
				body.moving = false;
				continue;
			}

			const auto&& offsetNormalized = offset.Normalized2d();
			const float&& delta = parentMovement.movementSpeed * body.speedMultiplier * deltaTime;

			transform.posLocal += offsetNormalized * delta;

			// Rotate smoothly.
			transform.rot = parentTransform.rot;
		}

		transform.posLocal.z = parentTransform.posGlobal.z - .05f;
	}
}
