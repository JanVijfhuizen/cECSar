#include <Factories/Items/SwordFactory.h>
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Factories/Implementations/StandardRendererImp.h"
#include "Components/RigidBody.h"
#include "Components/Collider.h"

void game::SwordFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.path = "Art/Sword.png";

	DefineImplementation<RigidBody>();
	DefineImplementation<Collider>();
}
