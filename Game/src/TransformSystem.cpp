#include <Systems/TransformSystem.h>
#include "Modules/BufferModule.h"
#include "Modules/JobConverterModule.h"

game::TransformSystem::~TransformSystem()
{
	delete [] _sortableIndexes;
}

void game::TransformSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_jobConverter = &_cecsar->GetModule<JobConverterModule>();

	_sortableIndexes = new int32_t[_cecsar->info.setCapacity];
	_transformBuffer = cecsar.GetModule<BufferModule<Transform>>().buffer;
}

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>& transforms)
{
	_jobConverter->ToLinearJobs(transforms.GetCount(),
		[this, &transforms]
	(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				auto& transformBuffer = _transformBuffer->operator[](i);
				auto& transform = transforms[i];

				if (transformBuffer.parent == -1)
				{
					transform.posGlobal = transformBuffer.posLocal;
					transform.rotGlobal = transformBuffer.rotLocal;
					continue;
				}

				utils::Vector3 worldPos = transformBuffer.posLocal;
				float worldRot = transformBuffer.rotLocal;

				int32_t parentIndex = transformBuffer.parent;

				while (parentIndex != -1)
				{
					auto& parent = _transformBuffer->Get(parentIndex);

					worldPos = parent.posLocal + worldPos.Rotate(parent.rotLocal);
					worldRot += parent.rotLocal;

					parentIndex = parent.parent;
				}

				transform.posGlobal = worldPos;
				transform.rotGlobal = utils::Mathf::ConstrainAngle(worldRot);
			}
		});
}
