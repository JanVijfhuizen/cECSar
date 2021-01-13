﻿#include <Factories/Humanoids/HandFactory.h>
#include <Components/Renderer.h>
#include <Components/Transform.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/Kinematic.h>
#include "Components/Animator.h"
#include "Components/Joint.h"

void game::HandFactory::OnInitializeCustom(cecsar::Cecsar&)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.prototype.count = 4;

	DefineImplementation<Kinematic>();
	DefineImplementation<Animator>();
	auto& joint = DefineImplementation<Joint>();
	joint.prototype.maxDistance = 16;
}
