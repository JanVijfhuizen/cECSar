#pragma once
#include "HeadFactory.h"
#include <Components/Renderer.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class RoninHeadFactory final : public HeadFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void RoninHeadFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		HeadFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/Ronin.png";
	}
}
