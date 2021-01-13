#pragma once
#include <Factories/Humanoids/LegFactory.h>
#include <Components/Renderer.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include "Components/Leg.h"

namespace game
{
	class OniLegFactory final : public LegFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void OniLegFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		LegFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/OniLeg.png";

		offset.x = 32;
		offset.y = -32;

		auto& leg = DefineImplementation<Leg>();
		leg.prototype.moveThreshold = 32;
	}
}
