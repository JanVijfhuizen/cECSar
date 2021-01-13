#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Factories/Humanoids/RoninHandFactory.h>
#include "RoninHeadFactory.h"

namespace game
{
	class RoninFactory final : public HumanoidFactory
	{
	private:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void RoninFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		HumanoidFactory::OnInitializeCustom(cecsar);

		DefineImplementation<CameraFollowTarget>();

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/RoninBody.png";

		auto& controller = DefineImplementation<Controller>();
		controller.prototype.type = ControllerType::player;

		SetHandFactoryImpl<RoninHandFactory>(cecsar);
		SetHeadFactoryImpl<RoninHeadFactory>(cecsar);
	}
}
