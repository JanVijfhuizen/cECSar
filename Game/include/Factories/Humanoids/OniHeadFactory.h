#pragma once
#include "HeadFactory.h"
#include <Components/Renderer.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class OniHeadFactory final : public HeadFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void OniHeadFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		HeadFactory::OnInitializeCustom(cecsar);

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/OniHead.png";
		offset.y = 28;
		offset.z = .02f;
	}
}
