#include <Systems/TransformSystem.h>

game::TransformSystem::~TransformSystem()
{
	delete [] _sortableIndexes;
}

void game::TransformSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_sortableIndexes = new int32_t[_cecsar->info.setCapacity];
}

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>& transforms)
{
	for (Transform& transform : transforms)
	{
		if (transform.parent == -1)
		{
			transform.posGlobal = transform.posLocal;
			transform.rotGlobal = transform.rot;
			continue;
		}

		utils::Vector3 worldPos{};
		float worldRot = 0;

		int32_t parentIndex = transform.parent;
		while(parentIndex != -1)
		{
			auto& parent = transforms.Get(parentIndex);
			const int32_t grandParentIndex = parent.parent;

			// Check if parent has been deleted.
			if (grandParentIndex != -1)
			{
				if (!transforms.Contains(grandParentIndex))
				{
					worldPos += parent.posGlobal;
					worldRot += parent.rot;

					parent.parent = -1;
					break;
				}
			}

			worldPos += parent.posLocal.Rotate(worldRot);
			worldRot += parent.rot;

			parentIndex = grandParentIndex;
		}

		transform.posGlobal = worldPos + transform.posLocal.Rotate(worldRot);
		transform.rotGlobal = utils::Mathf::ConstrainAngle(worldRot + transform.rot);
	}
}
