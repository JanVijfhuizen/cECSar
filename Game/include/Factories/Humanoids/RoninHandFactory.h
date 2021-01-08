#pragma once
#include <Factories/Humanoids/HandFactory.h>
#include <Components/Renderer.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class RoninHandFactory final : public HandFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void RoninHandFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		HandFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/Ronin.png";
	}
}
