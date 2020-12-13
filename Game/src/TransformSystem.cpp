#include <Systems/TransformSystem.h>
#include <SDL_stdinc.h>

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>& transforms)
{
	// Get root objects at the front.
	transforms.Sort(Sort);

	for (Transform& transform : transforms)
	{
		if (transform.parent == -1) 
		{
			transform.p4Global = transform.p4;
			transform.rotationGlobal = transform.rotation;
			continue;
		}

		auto& parent = transforms.Get(transform.parent);

		transform.rotationGlobal = transform.rotation + parent.rotationGlobal;

		const float halfC = M_PI / 180;
		const float rad = transform.rotationGlobal * halfC;

		const float sin = std::sinf(rad);
		const float cos = std::cosf(rad);

		const float xSin = transform.x * sin;
		const float xCos = transform.x * cos;

		const float ySin = transform.y * sin;
		const float yCos = transform.y * cos;

		transform.xGlobal = parent.xGlobal + xCos - ySin;
		transform.yGlobal = parent.yGlobal + xSin + yCos;
		transform.zGlobal = parent.zGlobal + transform.z;
	}
}

float game::TransformSystem::Sort(const Transform& transform, const int32_t index)
{
	return -transform.rDepth;
}
