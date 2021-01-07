#include <Factories/Humanoids/HumanoidFactory.h>
#include <Components/Collider.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/MovementComponent.h>
#include <Components/Controller.h>
#include <Components/RigidBody.h>

void game::HumanoidFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Renderer, StandardRendererImp>();
	DefineImplementation<Collider>();

	DefineImplementation<MovementComponent>();
	DefineImplementation<Controller>();
	DefineImplementation<RigidBody>();
}
