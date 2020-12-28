#include <Systems/TransformSystem.h>

game::Transform game::TransformSystem::ToWorld(const Transform& t, const utils::Vector3& p) const
{
	Transform world = t;
	world.position += p;

	const Transform* current = &t;
	while (current->parent != -1)
	{
		Transform& parent = _transforms->Get(current->parent);
		world.position = parent.position + world.position.Rotate(parent.rotation);
		world.rotation += parent.rotation;

		current = &parent;
	}

	return world;
}

game::Transform game::TransformSystem::ToLocal(const Transform& t, const utils::Vector3& p) const
{
	Transform local = t;
	local.position += p;

	const Transform* current = &t;
	while (t.parent != -1)
	{
		Transform& parent = _transforms->Get(current->parent);
		local.position = local.position.Rotate(-parent.rotation) - parent.position;
		local.rotation -= parent.rotation;

		current = &parent;
	}

	return local;
}

void game::TransformSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_transforms = &cecsar.GetSet<Transform>();
}

void game::TransformSystem::OnUpdate(utils::SparseSet<Transform>&)
{
}
