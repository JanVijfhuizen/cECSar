#pragma once
#include <Components/Renderer.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class RoninLegFactory final : public LegFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void RoninLegFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		LegFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/RoninHand.png";
	}
}
