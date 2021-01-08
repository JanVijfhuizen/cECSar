﻿#include <Factories/Humanoids/HumanoidFactory.h>
#include <Components/Collider.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/MovementComponent.h>
#include <Components/Controller.h>
#include <Components/RigidBody.h>

void game::HumanoidFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();

	DefineImplementation<Renderer, StandardRendererImp>();
	DefineImplementation<Collider>();

	DefineImplementation<MovementComponent>();
	DefineImplementation<Controller>();
	DefineImplementation<RigidBody>();

	SetHandFactoryImpl<HandFactory>(cecsar);
}

void game::HumanoidFactory::OnConstructionCustom(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	auto& transforms = cecsar.GetSet<Transform>();
	auto& renderers = cecsar.GetSet<Renderer>();

	// Add the hands.
	const auto hands = cecsar.AddEntity(2);
	for (int32_t i = 0; i < 2; ++i)
	{
		auto& handInfo = hands[i];
		_handFactory->Construct(cecsar, handInfo);

		auto& handTransform = transforms.Get(handInfo.index);
		handTransform.parent = info;

		auto offset = _handFactory->offset;
		offset.x *=(i == 0) * 2 - 1;
		handTransform.position = offset;

		auto& handRenderer = renderers.Get(handInfo.index);
		if(i == 1)
			handRenderer.flip = SDL_FLIP_HORIZONTAL;
	}
}
