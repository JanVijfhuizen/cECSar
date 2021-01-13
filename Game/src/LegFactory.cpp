#include "Factories/Humanoids/LegFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Factories/Implementations/StandardRendererImp.h"
#include "Components/Animator.h"
#include "Components/Joint.h"
#include "Components/Leg.h"

void game::LegFactory::OnInitializeCustom(cecsar::Cecsar&)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.prototype.count = 4;

	DefineImplementation<Animator>();
	DefineImplementation<Joint>();
	DefineImplementation<Leg>();
}
