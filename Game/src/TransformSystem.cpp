#include <Systems/TransformSystem.h>
#include "Modules/JobConverterModule.h"

game::TransformSystem::~TransformSystem()
{
	delete [] _sortableIndexes;
}

void game::TransformSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<Transform>::Initialize(cecsar);

	_sortableIndexes = new int32_t[cecsar.info.setCapacity];
	_transforms = &cecsar.GetSet<Transform>();
}

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>& transforms)
{
	GetJobModule().ToLinearJobs(transforms.GetCount(),[&transforms]
	(const int32_t start, const int32_t stop)
		{
			for (int32_t i = start; i < stop; ++i)
			{
				auto& transform = transforms[i];

				if (transform._parent == -1)
				{
					transform.posGlobal = transform.posLocal;
					transform.rotGlobal = transform.rotLocal;
					continue;
				}

				utils::Vector3 worldPos = transform.posLocal;
				float worldRot = transform.rotLocal;

				int32_t parentIndex = transform._parent;

				while (parentIndex != -1)
				{
					auto& parent = transforms.Get(parentIndex);

					worldPos = parent.posLocal + worldPos.Rotate(parent.rotLocal);
					worldRot += parent.rotLocal;

					parentIndex = parent._parent;
				}

				transform.posGlobal = worldPos;
				transform.rotGlobal = utils::Mathf::ConstrainAngle(worldRot);
			}
		});
}
