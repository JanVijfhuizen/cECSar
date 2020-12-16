#include "Systems/BodySystem.h"
#include "Modules/TimeModule.h"
#include "Factories/BodyFactory.h"

void game::BodySystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::BodySystem::OnUpdate(
	utils::SparseSet<BodyComponent>& bodies, utils::SparseSet<Transform>& transforms)
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

		// Update z position.
		transform.posLocal.z = parentTransform.posGlobal.z - .05f;

		const auto target4 = _mm_add_ps(transform.posGlobal.v4, body.offset.v4);
		utils::Vector3 offset(_mm_sub_ps(parentTransform.posGlobal.v4, target4));
		offset.z = 0;

		const float magnitude = offset.Magnitude();
		const utils::Vector3 offsetNormalized = offset.Normalized();

		// Move if it's too far away.
		if(!body.moving)
			if (magnitude > body.moveThreshold)
				body.moving = true;

		if (!body.moving)
			continue;

		// If the bodypart is close enough.
		if(magnitude < body.stoppingDistance)
		{
			body.moving = false;
			continue;
		}

		// Teleport if too far away.
		// Also rotate towards the correct direction.

		const auto&& p4Delta = _mm_set_ps1(deltaTime * body.moveSpeed);
		const utils::Vector3 dir(_mm_mul_ps(offsetNormalized.v4, p4Delta));

		transform.posLocal.v4 = _mm_add_ps(transform.posLocal.v4, dir.v4);
	}
}
