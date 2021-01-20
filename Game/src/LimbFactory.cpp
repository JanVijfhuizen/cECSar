#include "Factories/Humanoids/LimbFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Factories/Implementations/StandardRendererImp.h"
#include "Components/Kinematic.h"
#include "Components/Animator.h"
#include "Components/Joint.h"
#include "Components/RigidBody.h"

void game::LimbFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.prototype.count = 4;

	DefineImplementation<Kinematic>();
	DefineImplementation<Animator>();
	DefineImplementation<Joint>();
	
	auto& rigidBody = DefineImplementation<RigidBody>();
	rigidBody.prototype.weight = .75f;
}
