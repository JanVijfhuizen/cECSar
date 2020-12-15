#include "Systems/BodySystem.h"

void game::BodySystem::OnUpdate(
	utils::SparseSet<BodyComponent>& bodies, utils::SparseSet<Transform>& transforms)
{
	const auto iterator = bodies.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& body = bodies[i];
		if (body.parent == -1)
			continue;

		auto& transform = transforms.Get(iterator[i]);
		auto& parentTransform = transforms.Get(body.parent);

		const utils::Vector3 offset(_mm_sub_ps(transform.posGlobal.v4, parentTransform.posGlobal.v4));
		const float magnitude = offset.Magnitude();

		// Move outwards when too close, move inwards when too far.
		// Also base it on current movement type.
	}
}
