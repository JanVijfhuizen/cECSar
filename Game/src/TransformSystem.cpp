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
			transform.rotGlobal = transform.rotLocal;
			continue;
		}

		utils::Vector3 worldPos = transform.posLocal;
		float worldRot = transform.rotLocal;

		Transform* current = &transform;
		int32_t parentIndex = transform.parent;

		while(parentIndex != -1)
		{
			if(!transforms.Contains(parentIndex))
			{
				worldPos -= current->posLocal;
				worldRot -= current->rotLocal;

				worldPos += current->posGlobal;
				worldPos += current->rotGlobal;

				current->posLocal = worldPos;
				current->rotLocal = utils::Mathf::ConstrainAngle(worldRot);

				current->parent = -1;
				break;
			}

			auto& parent = transforms.Get(parentIndex);

			worldPos = parent.posLocal + worldPos.Rotate(parent.rotLocal);
			worldRot += parent.rotLocal;

			parentIndex = parent.parent;
			current = &parent;
		}

		transform.posGlobal = worldPos;
		transform.rotGlobal = utils::Mathf::ConstrainAngle(worldRot);
	}
}
