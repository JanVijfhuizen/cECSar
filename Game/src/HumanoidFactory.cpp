#include <Factories/Humanoids/HumanoidFactory.h>
#include <Components/Collider.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/MovementComponent.h>
#include <Components/Controller.h>
#include <Components/RigidBody.h>
#include "Components/Joint.h"
#include "Components/Leg.h"
#include "Components/Animator.h"

void game::HumanoidFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	DefineImplementation<Transform>();

	auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
	renderer.prototype.count = 4;

	DefineImplementation<Animator>();
	DefineImplementation<Collider>();

	DefineImplementation<MovementComponent>();
	DefineImplementation<Controller>();
	DefineImplementation<RigidBody>();

	SetHandFactoryImpl<HandFactory>(cecsar);
	SetHeadFactoryImpl<HeadFactory>(cecsar);
}

void game::HumanoidFactory::OnConstructionCustom(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	auto& transforms = cecsar.GetSet<Transform>();
	auto& renderers = cecsar.GetSet<Renderer>();
	auto& joints = cecsar.GetSet<Joint>();
	auto& legs = cecsar.GetSet<Leg>();

	// Construct the hands.
	const auto hands = cecsar.AddEntity(2);
	for (int32_t i = 0; i < 2; ++i)
	{
		auto& handInfo = hands[i];
		_handFactory->Construct(cecsar, handInfo);

		auto& joint = joints.Get(handInfo.index);
		joint.other = info;

		auto offset = _handFactory->offset;
		offset.x *= (i == 0) * 2 - 1;
		offset.y = -8;
		joint.offset = offset;

		auto& leg = legs.Get(handInfo.index);
		leg.root = info;
		leg.mirror = hands[1 - i];

		auto& handRenderer = renderers.Get(handInfo.index);
		if(i == 1)
			handRenderer.flip = SDL_FLIP_HORIZONTAL;
	}

	{
		// Construct the head.
		const auto headInfo = cecsar.AddEntity()[0];
		_headFactory->Construct(cecsar, headInfo);

		auto& headTransform = transforms.Get(headInfo.index);
		headTransform.parent = info;

		headTransform.position = _headFactory->offset;
	}
}
