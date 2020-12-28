#include <Systems/TransformSystem.h>
#include <stack>

game::Transform game::TransformSystem::ToWorld(const Transform& t, const utils::Vector3& p) const
{
	Transform world = t;
	world.position += p;

	const Transform* current = &t;
	while (current->parent.index != -1)
	{
		Transform& parent = _transforms->Get(current->parent.index);
		world.position = parent.position + world.position.Rotate(parent.rotation);
		world.rotation += parent.rotation;

		current = &parent;
	}

	return world;
}

game::Transform game::TransformSystem::ToLocal(const Transform& t, const utils::Vector3& p) const
{
	// I'm sure there is a better way to do it.
	// Too bad I'm not a mathmatician.

	struct Calc final
	{
		float rot{};
		utils::Vector3 pos{};
	};

	std::stack<Calc> stack;

	// Do the inverse calculations in the reverse order to get to the local position.
	const Transform* current = &t;
	while (current->parent.index != -1)
	{
		Transform& parent = _transforms->Get(current->parent.index);

		Calc c;
		c.pos = parent.position;
		c.rot = parent.rotation;
		stack.push(c);

		current = &parent;
	}

	Transform local{};
	local.position = p;

	while(!stack.empty())
	{
		const Calc c = stack.top();
		stack.pop();

		local.position = (local.position - c.pos).Rotate(-c.rot);
		local.rotation -= c.rot;
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
