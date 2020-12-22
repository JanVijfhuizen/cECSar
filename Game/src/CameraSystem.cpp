#include <Systems/CameraSystem.h>
#include <algorithm>
#include "Modules/RenderModule.h"
#include "Modules/TimeModule.h"
#include "Modules/BufferModule.h"

void game::CameraSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
	_timeModule = &cecsar.GetModule<TimeModule>();
	_transformBuffer = cecsar.GetModule<BufferModule<Transform>>().buffer;
}

void game::CameraSystem::OnUpdate(
	utils::SparseSet<CameraFollowTarget>& targets)
{
	if (!targets.GetCount())
		return;

	utils::Vector3 center;
	float xMin = std::numeric_limits<float>::max();
	float xMax = std::numeric_limits<float>::min();
	float yMin = std::numeric_limits<float>::max();
	float yMax = std::numeric_limits<float>::min();

	const auto dense = targets.GetDenseRaw();
	for (int32_t i = targets.GetCount() - 1; i >= 0; --i)
	{
		auto& transform = _transformBuffer->Get(dense[i]);
		auto position = transform.posGlobal;

		// Update bounds.
		xMin = std::min(xMin, position.x);
		yMin = std::min(yMin, position.y);
		xMax = std::min(xMax, position.x);
		yMax = std::min(yMax, position.y);

		center.v4 = _mm_add_ps(center.v4, position.v4);
	}

	center.v4 = _mm_div_ps(center.v4, _mm_set_ps1(targets.GetCount()));
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

	// X axis.
	if (abs(offset.x) >= _followThreshold.x)
		// Moving.
		if (abs(offset.x) <= _hardFollowThreshold.x)
		{
			const float xFollowSpeed = deltaTime * _followSpeed * (offset.x > 0 ? 1 : -1);
			cameraPosition.x -= xFollowSpeed;
		}
	// Teleporting.
		else
		{
			const float diff = abs(abs(offset.x) - _hardFollowThreshold.x);
			cameraPosition.x -= diff * (offset.x > 0 ? 1 : -1);
		}

	// Y axis.
	if (abs(offset.y) >= _followThreshold.y)
		// Moving.
		if (abs(offset.y) <= _hardFollowThreshold.y)
		{
			const float yFollowSpeed = deltaTime * _followSpeed * (offset.y > 0 ? 1 : -1);
			cameraPosition.y -= yFollowSpeed;
		}
	// Teleporting.
		else
		{
			const float diff = abs(abs(offset.y) - _hardFollowThreshold.y);
			cameraPosition.y -= diff * (offset.y > 0 ? 1 : -1);
		}

	const float hardFollowMagn = _hardFollowThreshold.Magnitude();
	const float offsetMagn = offset.Magnitude();

	const float magnLerp = offsetMagn / hardFollowMagn;
	const float convLerp = std::max(.0f, magnLerp - _movementZoomThreshold) / 
		(1.0f - _movementZoomThreshold);
	
	_renderModule->zoom = 1 + convLerp * _movementZoomMultiplier;
}
