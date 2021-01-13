#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/RigidBody.h>
#include <Components/Collider.h>
#include <Factories/Humanoids/OniHandFactory.h>
#include "OniHeadFactory.h"

namespace game
{
	class OniFactory final : public HumanoidFactory
	{
	private:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void OniFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		HumanoidFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/OniBody.png";

		auto& rigidBody = DefineImplementation<RigidBody>();
		rigidBody.prototype.weight = 1.5f;

		auto& collider = DefineImplementation<Collider>();

		auto circle = Collider::Circle();
		circle.radius *= 3;
		collider.prototype.type = circle;

		SetHandFactoryImpl<OniHandFactory>(cecsar);
		SetHeadFactoryImpl<OniHeadFactory>(cecsar);
	}
}
