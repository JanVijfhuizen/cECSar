#include <Systems/HandSystem.h>
#include <algorithm>

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

		const auto rootPos = transform.posGlobal - 
			(transform.posLocal - hand.offset).Rotate(transform.rotGlobal);
		const auto offset = targetTransform.posGlobal - rootPos;

		const auto dir = offset.Normalized2d();
		const float distance = offset.Magnitude2d();

		transform.posLocal = dir * std::min(distance, hand.maxDistance);
		transform.posLocal = hand.offset + transform.posLocal.Rotate(-transform.rotGlobal);
	}
}
