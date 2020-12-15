#include <Systems/TransformSystem.h>
#include <SDL_stdinc.h>

void game::TransformSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
}

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>& transforms)
{
	// Get root objects at the front.
	transforms.Sort(SortDepth);

	ClearHangingObjects(transforms);
	UpdateGlobalPositions(transforms);
}

void game::TransformSystem::ClearHangingObjects(utils::SparseSet<Transform>& transforms)
{
	const auto iterator = transforms.GetDenseIterator();
	for (int32_t i = transforms.GetCount() - 1; i >= 0; --i)
	{
		auto& transform = transforms[i];
		if (transform.parent == -1)
			continue;

		if (!transforms.Contains(transform.parent))
			_cecsar->RemoveEntity(iterator[i]);
	}
}

void game::TransformSystem::UpdateGlobalPositions(utils::SparseSet<Transform>& transforms)
{
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

float game::TransformSystem::SortDepth(const Transform& transform, const int32_t index)
{
	return -transform.rDepth;
}