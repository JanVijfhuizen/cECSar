#include <Systems/MovementSystem.h>
#include "Modules/TimeModule.h"
#include <iostream>
#include "Modules/JobConverterModule.h"

void game::MovementSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
	_jobConverter = &cecsar.GetModule<JobConverterModule>();
}

void game::MovementSystem::OnUpdate(
	utils::SparseSet<Transform>& transforms, utils::SparseSet<Controller>& controllers,
	utils::SparseSet<MovementComponent>& movementComponents)
{
	const auto deltaTime = _timeModule->GetDeltaTime();
	const auto iterator = movementComponents.GetDenseIterator();

	_jobConverter->ToLinearJobs(iterator.GetCount(),
		[deltaTime, &movementComponents, &controllers, &transforms, &iterator]
		(const int32_t start, const int32_t stop)
		{
			for (int32_t i = stop - 1; i >= start; --i)
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
		});

	_jobConverter->Wait();
}
