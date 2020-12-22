#include <Systems/MovementSystem.h>
#include "Modules/TimeModule.h"
#include <iostream>
#include "Modules/JobConverterModule.h"
#include "Modules/BufferModule.h"

void game::MovementSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_timeModule = &cecsar.GetModule<TimeModule>();
	_jobConverter = &cecsar.GetModule<JobConverterModule>();

	_transformBuffer = cecsar.GetModule<BufferModule<Transform>>().buffer;
	_controllerBuffer = cecsar.GetModule<BufferModule<Controller>>().buffer;
}

void game::MovementSystem::OnUpdate(utils::SparseSet<MovementComponent>& movementComponents)
{
	const auto deltaTime = _timeModule->GetDeltaTime();
	const auto iterator = movementComponents.GetDenseIterator();

	auto& transforms = *_transformBuffer;
	auto& controllers = *_controllerBuffer;

	_jobConverter->ToLinearJobs(iterator.GetCount(),
		[deltaTime, &movementComponents, &controllers, &transforms, &iterator]
		(const int32_t start, const int32_t stop)
		{
			for (int32_t i = stop - 1; i >= start; --i)
			{
				const int32_t index = iterator[i];

				auto& movementComponent = movementComponents[i];
				auto& controller = controllers.Get(index);
				auto& transform = transforms.Get(index);

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
				transform.rotLocal = utils::Vector3::RotateTowards2d(
					transform.rotLocal, target, delta);
			}
		});
}
