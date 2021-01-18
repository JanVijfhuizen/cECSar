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
	SetLegFactoryImpl<LegFactory>(cecsar);
	SetHeadFactoryImpl<HeadFactory>(cecsar);
}

void game::HumanoidFactory::OnConstructionCustom(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	auto& transforms = cecsar.GetSet<Transform>();
	auto& legComponents = cecsar.GetSet<Leg>();

	// Construct the hands and legs.
	const auto hands = ConstructLimbPair(cecsar, *_handFactory, info);
	const auto legs = ConstructLimbPair(cecsar, *_legFactory, info);

	// Assign leg mirrors.
	for (int32_t i = 0; i < 2; ++i)
	{
		auto& legInfo = legs[i];
		auto& leg = legComponents.Get(legInfo.index);

		leg.root = info;
		leg.mirror = legs[1 - i];
	}

	// Construct the head.
	const auto headInfo = cecsar.AddEntity()[0];
	_headFactory->Construct(cecsar, headInfo);

	auto& headTransform = transforms.Get(headInfo.index);
	headTransform.parent = info;
	headTransform.position = _headFactory->offset;

	OnHandsConstruction(cecsar, hands);
	OnLegsConstruction(cecsar, legs);
	OnHeadConstruction(cecsar, headInfo);
}

void game::HumanoidFactory::OnHandsConstruction(
	cecsar::Cecsar&, std::shared_ptr<cecsar::EntityInfo[]>)
{
}

void game::HumanoidFactory::OnLegsConstruction(
	cecsar::Cecsar&, std::shared_ptr<cecsar::EntityInfo[]>)
{
}

void game::HumanoidFactory::OnHeadConstruction(
	cecsar::Cecsar&, cecsar::EntityInfo)
{
}

std::shared_ptr<cecsar::EntityInfo[]> game::HumanoidFactory::ConstructLimbPair(
	cecsar::Cecsar& cecsar, LimbFactory& factory, const cecsar::EntityInfo& info)
{
	auto& joints = cecsar.GetSet<Joint>();
	auto& renderers = cecsar.GetSet<Renderer>();

	const auto limbs = cecsar.AddEntity(2);

	for (int32_t i = 0; i < 2; ++i)
	{
		auto& limbInfo = limbs[i];
		factory.Construct(cecsar, limbInfo);

		auto& joint = joints.Get(limbInfo.index);
		joint.other = info;

		auto offset = factory.offset;
		offset.x *= (i == 0) * 2 - 1;
		joint.offset = offset;

		auto& renderer = renderers.Get(limbInfo.index);
		if (i == 1)
			renderer.flip = SDL_FLIP_HORIZONTAL;
	}

	return limbs;
}
