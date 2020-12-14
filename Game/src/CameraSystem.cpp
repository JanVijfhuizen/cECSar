﻿#include <Systems/CameraSystem.h>
#include <algorithm>
#include "Modules/RenderModule.h"
#include "Modules/TimeModule.h"

void game::CameraSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
	_timeModule = &cecsar.GetModule<TimeModule>();
}

void game::CameraSystem::OnUpdate(
	utils::SparseSet<CameraFollowTarget>& targets, utils::SparseSet<Transform>& transforms)
{
	if (!targets.GetCount())
		return;

	utils::Vector3 center;
	float xMin = std::numeric_limits<float>::max();
	float xMax = std::numeric_limits<float>::min();
	float yMin = std::numeric_limits<float>::max();
	float yMax = std::numeric_limits<float>::min();

	const auto iterator = targets.GetDenseIterator();
	for (int32_t i : iterator)
	{
		auto& transform = transforms.Get(i);
		auto position = transform.posGlobal;

		// Update bounds.
		xMin = std::min(xMin, position.x);
		yMin = std::min(yMin, position.y);
		xMax = std::min(xMax, position.x);
		yMax = std::min(yMax, position.y);

		center.v4 = _mm_add_ps(center.v4, position.v4);
	}

	center.v4 = _mm_div_ps(center.v4, _mm_set_ps1(iterator.GetCount()));
	center.x -= _renderModule->SCREEN_WIDTH / 2;
	center.y -= _renderModule->SCREEN_HEIGHT / 2;

	UpdatePosition(center);
}

void game::CameraSystem::UpdatePosition(const utils::Vector3 target) const
{
	auto& cameraPosition = _renderModule->cameraTransform.posLocal;
	const utils::Vector3 offset(
		cameraPosition.x - target.x,
		cameraPosition.y - target.y);

	const float deltaTime = _timeModule->GetDeltaTime();

	// X axis.
	if (abs(offset.x) >= _followThreshold.x)
		// Moving.
		if (abs(offset.x) <= _hardFollowThreshold.x)
		{
			cameraPosition.x -= deltaTime * _followSpeed * (offset.x > 0 ? 1 : -1);
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
			cameraPosition.y -= deltaTime * _followSpeed * (offset.y > 0 ? 1 : -1);
		}
	// Teleporting.
		else
		{
			const float diff = abs(abs(offset.y) - _hardFollowThreshold.y);
			cameraPosition.y -= diff * (offset.y > 0 ? 1 : -1);
		}

	const float zoomLerp = offset.Magnitude() / _hardFollowThreshold.Magnitude();
	_renderModule->zoom = 1 + zoomLerp * _movementZoomMultiplier;
}
