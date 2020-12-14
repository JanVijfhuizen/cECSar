#include <Systems/MovementSystem.h>
#include <SDL_stdinc.h>
#include "Modules/TimeModule.h"

void game::MovementSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::MovementSystem::OnUpdate(
	utils::SparseSet<Transform>& transforms, utils::SparseSet<Controller>& controllers,
	utils::SparseSet<MovementComponent>& movementComponents)
{
	auto halfP = 180 / M_PI;
	const auto deltaTime = _timeModule->GetDeltaTime();

	const auto iterator = movementComponents.GetDenseIterator();
	for (int i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& movementComponent = movementComponents[i];
		auto& controller = controllers.Get(iterator[i]);
		auto& transform = transforms.Get(iterator[i]);

		const float xDelta = controller.xDir * movementComponent.movementSpeed * deltaTime;
		const float yDelta = controller.yDir * movementComponent.movementSpeed * deltaTime;

		transform.posLocal.x += xDelta;
		transform.posLocal.y += yDelta;

		const bool rotate = abs(xDelta) > 0 || abs(yDelta) > 0;
		if (!rotate)
			continue;

		float targetDegrees = atan2f(controller.xDir, -controller.yDir) * halfP;
		if (targetDegrees < 0)
			targetDegrees += 360;

		const float currentDegrees = transform.rotGlobal - floor(transform.rotGlobal / 360) * 360;

		bool dir = currentDegrees > targetDegrees;
		const bool reverse = std::abs(currentDegrees - targetDegrees) < 180;
		if (reverse)
			dir = !dir;

		const float delta = movementComponent.rotationSpeed * (dir * 2 - 1) * deltaTime;
		transform.rot += delta;
	}
}
