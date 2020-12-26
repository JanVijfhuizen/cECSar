#pragma once
#include "HumanoidFactory.h"
#include "Implementations/StandardRendererImp.h"

namespace game
{
	class OniFactory final : public HumanoidFactory
	{
	protected:
		IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
	};

	inline IFactoryImp<Renderer>* OniFactory::SetRenderImp(cecsar::Cecsar& cecsar)
	{
		auto renderer = new StandardRendererImp;
		renderer->path = "Art/Oni.png";
		renderer->blueprint.count = 6;
		return renderer;
	}
}
