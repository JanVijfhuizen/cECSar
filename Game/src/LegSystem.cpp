#include "Systems/LegSystem.h"
#include "Modules/TimeModule.h"
#include <algorithm>

void game::LegSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<LegComponent, MovementComponent, Transform>::Initialize(cecsar);

	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::LegSystem::OnUpdate(
	utils::SparseSet<LegComponent>& legs, utils::SparseSet<MovementComponent>& movements, 
	utils::SparseSet<Transform>& transforms)
{
	const float deltaTime = _timeModule->GetDeltaTime();
	const auto dense = legs.GetDenseRaw();

	GetJobModule().ToLinearJobs(legs.GetCount(),
		[&legs, &transforms, &movements, dense, deltaTime]
	(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				auto& leg = legs[i];
				if (leg.parent == -1)
					continue;

				auto& transform = transforms.Get(dense[i]);
				auto& parentTransform = transforms.Get(leg.parent);
				const auto& parentMovement = movements.Get(leg.parent);

				// Calculate target position.
				const auto rotatedTargetOffset = leg.offset.Rotate(parentTransform.rotGlobal);
				auto targetPosition = parentTransform.posGlobal + rotatedTargetOffset;
				const utils::Vector3 offset = targetPosition - transform.posGlobal;

				// Calculate offset.
				const float magnitude = offset.Magnitude2d();

				// Move if it's too far away and the linked bodypart isn't moving either.
				if (!leg.moving)
					if (magnitude > leg.softMoveThreshold)
					{
						if (leg.other == -1 || magnitude > leg.hardMoveThreshold)
							leg.moving = true;
						else if (!legs.Get(leg.other).moving)
							leg.moving = true;
					}

				// Somehow it doesnt center.
				if (leg.moving)
				{
					// If the bodypart is close enough.
					if (magnitude < leg.stoppingDistance)
					{
						leg.moving = false;
						continue;
					}

					const auto&& offsetNormalized = offset.Normalized2d();
					const float&& delta = parentMovement.movementSpeed * 
						leg.speedMultiplier * deltaTime;

					transform.posLocal += offsetNormalized * 
						std::min(delta, magnitude);
					transform.rotLocal = parentTransform.rotGlobal;
				}

				transform.posLocal.z = parentTransform.posGlobal.z - .05f;
			}
		});
}
