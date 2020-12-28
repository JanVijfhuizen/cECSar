#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class RoninFactory final : public HumanoidFactory
	{
	protected:
		inline IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
	};

	inline IFactoryImp<Renderer>* RoninFactory::SetRenderImp(cecsar::Cecsar& cecsar)
	{
		auto renderer = new StandardRendererImp;
		renderer->path = "Art/Ronin.png";
		renderer->prototype.count = 6;
		return renderer;
	}
}
