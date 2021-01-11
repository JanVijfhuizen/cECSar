#include <Factories/Humanoids/HeadFactory.h>
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Factories/Implementations/StandardRendererImp.h"
#include "Components/Kinematic.h"

void game::HeadFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.prototype.count = 6;

	DefineImplementation<Kinematic>();
}
