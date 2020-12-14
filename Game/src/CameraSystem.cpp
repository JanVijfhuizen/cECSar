#include <Systems/CameraSystem.h>
#include <algorithm>
#include "Modules/RenderModule.h"

void game::CameraSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
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

	const float xMul = abs(xMin - xMax);
	const float yMul = abs(yMin - yMax);

	_renderModule->cameraTransform.posLocal = center;
}
