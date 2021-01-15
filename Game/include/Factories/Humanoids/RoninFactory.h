#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Factories/Humanoids/RoninHandFactory.h>
#include <Factories/Humanoids/RoninLegFactory.h>
#include <Factories/Humanoids/RoninHeadFactory.h>

namespace game
{
	class RoninFactory final : public HumanoidFactory
	{
	private:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
		inline void OnHandsConstruction(cecsar::Cecsar& cecsar, 
			std::shared_ptr<cecsar::EntityInfo[]> info) override;
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
		SetLegFactoryImpl<RoninLegFactory>(cecsar);
		SetHeadFactoryImpl<RoninHeadFactory>(cecsar);
	}

	inline void RoninFactory::OnHandsConstruction(
		cecsar::Cecsar& cecsar, std::shared_ptr<cecsar::EntityInfo[]> info)
	{
		auto& controllers = cecsar.GetSet<Controller>();
		for (int32_t i = 0; i < 2; ++i)
		{
			const int32_t handIndex = info[i].index;
			auto& controller = controllers.Get(handIndex);

			controller.type = ControllerType::player;
		}
	}
}
