﻿#include <Factories/Humanoids/HandFactory.h>
#include <Components/Renderer.h>
#include <Components/Transform.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/Kinematic.h>

void game::HandFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();
	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();

	// Testing.
	renderer.path = "Art/Oni.png";
	renderer.prototype.count = 6;
	renderer.prototype.index = 2;

	DefineImplementation<Kinematic>();
}
