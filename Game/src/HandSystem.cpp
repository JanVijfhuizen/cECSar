#include <Systems/HandSystem.h>

void game::HandSystem::OnUpdate(utils::SparseSet<HandComponent>& hands, 
	utils::SparseSet<Transform>& transforms)
{
	const auto iterator = hands.GetDenseIterator();
	for (int32_t i = iterator.GetCount() - 1; i >= 0; --i)
	{
		auto& hand = hands[i];
		if (!hand.follow)
			continue;

		auto& transform = transforms.Get(iterator[i]);
		const auto offset = transform.posGlobal - hand.target;
		if (offset.SquaredLength() < 1e-5)
			continue;

		const auto dir = offset.Normalized2d();
		float distance = offset.Magnitude2d();

		// Rotate and move hand towards objective.
		// Keep in mind that the local position is rotated.
	}
}
