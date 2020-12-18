#include <Systems/HandSystem.h>
#include <algorithm>
#include "Helpers/TransformHelper.h"

void game::HandSystem::OnUpdate(utils::SparseSet<HandComponent>& hands, 
	utils::SparseSet<Transform>& transforms)
{
	const auto iterator = hands.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& hand = hands[i];
		if (hand.target == -1)
			continue;

		auto& transform = transforms.Get(iterator[i]);
		auto& targetTransform = transforms.Get(hand.target);

		const auto rootPos = TransformHelper::ToWorld(transform, hand.offset);
		const auto offset = targetTransform.posGlobal - rootPos;

		auto dir = offset.Normalized2d();
		const float distance = offset.Magnitude2d();

		dir *= std::min(distance, hand.maxDistance);
		transform.posLocal = hand.offset + dir.Rotate(-transform.rotGlobal);
	}
}
