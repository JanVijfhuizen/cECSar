#include <Systems/MovementSystem.h>
#include <SDL_stdinc.h>
#include "Modules/TimeModule.h"
#include <algorithm>
#include <iostream>
#include "Utils/Mathf.h"

void game::MovementSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::MovementSystem::OnUpdate(
	utils::SparseSet<Transform>& transforms, utils::SparseSet<Controller>& controllers,
	utils::SparseSet<MovementComponent>& movementComponents)
{
	const float halfP = 180 / M_PI;
	const auto deltaTime = _timeModule->GetDeltaTime();

	const auto iterator = movementComponents.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& movementComponent = movementComponents[i];
		auto& controller = controllers.Get(iterator[i]);
		auto& transform = transforms.Get(iterator[i]);
		
		const float deltaSpeed = movementComponent.movementSpeed * deltaTime;
		const auto input = utils::Vector3(controller.xDir, controller.yDir, 1);
		const auto normalized = input.Normalized() * deltaSpeed;

		transform.posLocal.x += normalized.x;
		transform.posLocal.y += normalized.y;

		const bool rotate = abs(normalized.x) > 0 || abs(normalized.y) > 0;
		if (!rotate)
			continue;

		const float delta = movementComponent.rotationSpeed * deltaTime;
		const auto target = utils::Vector3(controller.xDir, controller.yDir);
		transform.rot = utils::Vector3::RotateTowards2d(transform.rot, target, delta);
	}
}
