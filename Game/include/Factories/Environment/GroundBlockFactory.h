#pragma once
#include <Factories/StandardFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>
#include <Components/Collider.h>

namespace game
{
	class GroundBlockFactory final : public StandardFactory
	{
	protected:
		inline void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
	};

	inline void GroundBlockFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
	{
		DefineImplementation<Transform>();

		auto& renderer = DefineImplementation<Renderer, StandardRendererImp>();
		renderer.path = "Art/GroundBlock.png";

		auto& collider = DefineImplementation<Collider>();
		collider.prototype.targetMask = 0;
	}
}
