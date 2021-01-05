#include <Factories/Humanoids/HumanoidFactory.h>

game::HumanoidFactory::~HumanoidFactory()
{
	delete _movementImp;
	delete _controllerImp;
	delete _rigidBodyImp;
}

void game::HumanoidFactory::Initialize(cecsar::Cecsar& cecsar)
{
	StandardFactory::Initialize(cecsar);

	_movementImp = SetMovementImp(cecsar);
	_controllerImp = SetControllerImp(cecsar);
	_rigidBodyImp = SetRigidBodyImp(cecsar);
}

void game::HumanoidFactory::OnConstruction(cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	StandardFactory::OnConstruction(cecsar, info);
	const int32_t index = info.index;

	auto& movement = cecsar.AddComponent<MovementComponent>(index);
	auto& controller = cecsar.AddComponent<Controller>(index);
	auto& rigidBody = cecsar.AddComponent<RigidBody>(index);

	_movementImp->OnConstruction(cecsar, movement, index);
	_controllerImp->OnConstruction(cecsar, controller, index);
	_rigidBodyImp->OnConstruction(cecsar, rigidBody, index);
}

game::IFactoryImp<game::MovementComponent>* game::HumanoidFactory::SetMovementImp(
	cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<MovementComponent>;
}

game::IFactoryImp<game::Controller>* game::HumanoidFactory::SetControllerImp(
	cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<Controller>;
}

game::IFactoryImp<game::RigidBody>* game::HumanoidFactory::SetRigidBodyImp(
	const cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<RigidBody>;
}
