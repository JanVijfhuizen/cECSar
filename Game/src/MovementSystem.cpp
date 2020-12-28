#include <Systems/MovementSystem.h>
#include "Modules/TimeModule.h"
#include <iostream>
#include "Modules/JobConverterModule.h"
#include "Modules/BufferModule.h"

void game::MovementSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<MovementComponent, Transform, Controller>::Initialize(cecsar);
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::MovementSystem::OnUpdate(
	utils::SparseSet<MovementComponent>& movementComponents, 
	utils::SparseSet<Transform>& transforms,
	utils::SparseSet<Controller>& controllers)
{
	const auto deltaTime = _timeModule->GetDeltaTime();
	const auto dense = movementComponents.GetDenseRaw();

	auto& jobModule = GetJobModule();

	jobModule.ToLinearJobs(movementComponents.GetCount(),
		[deltaTime, dense, &movementComponents, &transforms, &controllers]
		(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				const int32_t index = dense[i];

				auto& movementComponent = movementComponents[i];
				auto& controller = controllers.Get(index);
				auto& transform = transforms.Get(index);

				// Move smoothly based on delta and controller input.
				const float deltaSpeed = movementComponent.movementSpeed * deltaTime;
				const auto input = utils::Vector3(controller.xDir, controller.yDir, 1);
				const auto normalized = input.Normalized() * deltaSpeed;

				transform.position.x += normalized.x;
				transform.position.y += normalized.y;

				// Rotate towards moving direction.
				const bool rotate = abs(normalized.x) > 0 || abs(normalized.y) > 0;
				if (!rotate)
					continue;

				const float delta = movementComponent.rotationSpeed * deltaTime;
				const auto target = utils::Vector3(controller.xDir, controller.yDir);
				transform.rotation = utils::Vector3::RotateTowards2d(
					transform.rotation, target, delta);

			}
		});

	jobModule.Wait();
}
