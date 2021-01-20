#include "Systems/HandSystem.h"

void game::HandSystem::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	_hands = &cecsar.GetSet<Hand>();
	_controllers = &cecsar.GetSet<Controller>();
	_joints = &cecsar.GetSet<Joint>();

	_cecsar->GetSystem<CollisionSystem>().Subscribe(*this);
}

void game::HandSystem::OnUpdate(
	utils::SparseSet<Hand>& hands,
	utils::SparseSet<Controller>& controllers,
	utils::SparseSet<Joint>& joints)
{

}

void game::HandSystem::OnNotify(const HitInfo& info)
{
	const auto& instance = info.instance;
	const int32_t index = instance.index;

	if (!_hands->Contains(index))
		return;

	auto& hand = _hands->Get(index);
	const auto& controller = _controllers->Get(index);

	const auto& handType = hand.type;

	switch (handType) 
	{ 
	case Hand::Type::Left:
		if (!controller.lMouse)
			return;
		break;
	case Hand::Type::Right:
		if (!controller.rMouse)
			return;
		break;
	case Hand::Type::Both:
		if (!controller.lMouse && !controller.rMouse)
			return;
		break;
	}

	if (_cecsar->IsEntityValid(hand.equipped))
		return;

	const auto& other = info.other;
	const int32_t otherIndex = other.index;

	// Set equipped.
	hand.equipped = 
	{ 
		otherIndex,
		_cecsar->GetEntity(otherIndex)
	};

	Joint&& joint
	{
		{
			index, 
			_cecsar->GetEntity(index)
		}
	};

	_joints->Insert(otherIndex, joint);
}
