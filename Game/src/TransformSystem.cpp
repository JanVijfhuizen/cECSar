#include <Systems/TransformSystem.h>

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

		auto& parent = transforms[transform.parent];
		transform.p4Global = _mm_add_ps(transform.p4, parent.p4Global);
		transform.rotationGlobal = transform.rotation + parent.rotationGlobal;
	}
}

float game::TransformSystem::Sort(const Transform& transform, const int32_t index)
{
	return -transform.depth;
}
