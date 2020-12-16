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

		const auto offset = targetTransform.posGlobal - transform.posGlobal;
		if (offset.SquaredLength() < 1e-3)
			continue;

		const auto dir = offset.Normalized2d();
		const float distance = offset.Magnitude2d();

		transform.posLocal = dir * std::min(distance * .9f, hand.maxDistance);
		transform.posLocal = hand.offset + transform.posLocal.Rotate(-transform.rotGlobal);
	}
}
