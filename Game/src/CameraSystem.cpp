#include <Systems/CameraSystem.h>
#include <algorithm>
#include "Modules/RenderModule.h"
#include "Modules/TimeModule.h"

void game::CameraSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::CameraSystem::OnUpdate(
	utils::SparseSet<CameraFollowTarget>& targets, 
	utils::SparseSet<Transform>& transforms)
{
	if (!targets.GetCount())
		return;

	// Get the center position for the camera.
	utils::Vector3 center;
	float xMin = std::numeric_limits<float>::max();
	float xMax = std::numeric_limits<float>::min();
	float yMin = std::numeric_limits<float>::max();
	float yMax = std::numeric_limits<float>::min();

	// Add to center and stretch the boundaries.
	const auto dense = targets.GetDenseRaw();
	for (int32_t i = targets.GetCount() - 1; i >= 0; --i)
	{
		auto& transform = transforms.Get(dense[i]);
		auto& position = transform.position;

		// Update bounds.
		xMin = std::min(xMin, position.x);
		yMin = std::min(yMin, position.y);
		xMax = std::min(xMax, position.x);
		yMax = std::min(yMax, position.y);

		center.v4 = _mm_add_ps(center.v4, position.v4);
	}

	// Average the target position.
	center /= targets.GetCount();
	center.x -= _renderModule->SCREEN_WIDTH / 2;
	center.y -= _renderModule->SCREEN_HEIGHT / 2;

	UpdatePosition(center);
}

void game::CameraSystem::UpdatePosition(const utils::Vector3& target) const
{
	auto& cameraPosition = _renderModule->cameraPos;
	const utils::Vector3 offset(
		cameraPosition.x - target.x,
		cameraPosition.y - target.y);

	const float deltaTime = _timeModule->GetDeltaTime();

	const int32_t xDir = offset.x > 0 ? 1 : -1;
	const int32_t yDir = offset.y > 0 ? 1 : -1;

	// X axis.
	if (abs(offset.x) >= _followThreshold.x)
		// Moving.
		if (abs(offset.x) <= _hardFollowThreshold.x)
		{
			
			const float xFollowSpeed = deltaTime * _followSpeed * xDir;
			cameraPosition.x -= xFollowSpeed;
		}
	// Teleporting.
		else
		{
			const float diff = abs(abs(offset.x) - _hardFollowThreshold.x);
			cameraPosition.x -= diff * xDir;
		}

	// Y axis.
	if (abs(offset.y) >= _followThreshold.y)
		// Moving.
		if (abs(offset.y) <= _hardFollowThreshold.y)
		{
			const float yFollowSpeed = deltaTime * _followSpeed * yDir;
			cameraPosition.y -= yFollowSpeed;
		}
	// Teleporting.
		else
		{
			const float diff = abs(abs(offset.y) - _hardFollowThreshold.y);
			cameraPosition.y -= diff * yDir;
		}
}
