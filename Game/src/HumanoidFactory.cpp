#include <Factories/Humanoids/HumanoidFactory.h>

game::HumanoidFactory::~HumanoidFactory()
{
	delete _movementImp;
	delete _controllerImp;
}

void game::HumanoidFactory::Initialize(cecsar::Cecsar& cecsar)
{
	StandardFactory::Initialize(cecsar);
	_movementImp = SetMovementImp(cecsar);
	_controllerImp = SetControllerImp(cecsar);
}

void game::HumanoidFactory::OnConstruction(cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	StandardFactory::OnConstruction(cecsar, info);

	auto& movement = cecsar.AddComponent<MovementComponent>(info.index);
	auto& controller = cecsar.AddComponent<Controller>(info.index);

	_movementImp->OnConstruction(cecsar, movement, info.index);
	_controllerImp->OnConstruction(cecsar, controller, info.index);
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
