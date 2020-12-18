#include <Systems/TransformSystem.h>
#include <SDL_stdinc.h>
#include "Sorter.h"
#include "Utils/Utils.h"

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
	SortIndexes(transforms);
	ClearHangingObjects(transforms);

	const float halfC = M_PI / 180;

	for (Transform& transform : transforms)
	{
		if (transform.parent == -1)
		{
			transform.posGlobal = transform.posLocal;
			transform.rotGlobal = transform.rot;
			continue;
		}

		auto& parent = transforms.Get(transform.parent);
		transform.rotGlobal = transform.rot + parent.rotGlobal;

		const float rad = transform.rotGlobal * halfC;
		const float sin = std::sinf(rad);
		const float cos = std::cosf(rad);

		const auto local = transform.posLocal;

		const float xSin = local.x * sin;
		const float xCos = local.x * cos;

		const float ySin = local.y * sin;
		const float yCos = local.y * cos;

		transform.posGlobal.x = parent.posGlobal.x + xCos - ySin;
		transform.posGlobal.y = parent.posGlobal.y + xSin + yCos;
		transform.posGlobal.z = parent.posGlobal.z + local.z;
	}
}

void game::TransformSystem::SortIndexes(utils::SparseSet<Transform>& transforms) const
{
	// Fill indexes cache with ordered indexes.
	const auto fillMethod = [](const int32_t index)
	{
		return index;
	};
	utils::Utils<int32_t>::Fill(_sortableIndexes, 0, transforms.GetCount(), fillMethod);

	// Get root objects at the front.
	const auto sortingMethod = [&transforms](const int32_t index)
	{
		return transforms[index].rDepth;
	};
	utils::Sorter<int32_t>::Sort(_sortableIndexes, 0, transforms.GetCount(), sortingMethod);
}

void game::TransformSystem::ClearHangingObjects(utils::SparseSet<Transform>& transforms) const
{
	// Remove all children whose parents are removed.
	// Doing this every frame is not great, but I haven't found a way to make this "safe" yet.
	const auto iterator = transforms.GetDenseIterator();
	for (int32_t i = transforms.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = _sortableIndexes[i];
		auto& transform = transforms[index];
		if (transform.parent == -1)
			continue;

		if (!transforms.Contains(transform.parent))
			_cecsar->RemoveEntity(iterator[index]);
	}
}