#include <Systems/HandSystem.h>
#include <algorithm>
#include "Helpers/TransformHelper.h"
#include "Modules/BufferModule.h"

void game::HandSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<HandComponent, Transform>::Initialize(cecsar);

	_transformBuffer = cecsar.GetModule<BufferModule<Transform>>().buffer;
}

void game::HandSystem::OnUpdate(utils::SparseSet<HandComponent>& hands, 
	utils::SparseSet<Transform>& transforms)
{
	const auto dense = hands.GetDenseRaw();

	GetJobModule().ToLinearJobs(hands.GetCount(),
		[this, &hands, &transforms, dense]
	(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				auto& hand = hands[i];
				if (hand.target == -1)
					continue;

				const int32_t index = dense[i];

				auto& transform = _transformBuffer->Get(index);
				auto& targetTransform = _transformBuffer->Get(hand.target);

				const auto rootPos = TransformHelper::ToWorld(transform, hand.offset);
				const auto offset = targetTransform.posGlobal - rootPos;

				auto dir = offset.Normalized2d();
				const float distance = offset.Magnitude2d();

				dir *= std::min(distance, hand.maxDistance);

				const auto posLocal = hand.offset + dir.Rotate(-transform.rotGlobal);
				transforms.Get(index).posLocal = posLocal;
			}
		});
}
