#pragma once
#include <Factories/StandardFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class GroundBlockFactory final : public StandardFactory
	{
	protected:
		inline IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
		IFactoryImp<Collider>* SetColliderImp(cecsar::Cecsar& cecsar) override;
	};

	inline IFactoryImp<Renderer>* GroundBlockFactory::SetRenderImp(cecsar::Cecsar& cecsar)
	{
		const auto renderer = new StandardRendererImp;
		renderer->path = "Art/GroundBlock.png";
		return renderer;
	}

	inline IFactoryImp<Collider>* GroundBlockFactory::SetColliderImp(cecsar::Cecsar& cecsar)
	{
		const auto collider = new IFactoryImp<Collider>;
		collider->prototype.targetMask = 0;
		return collider;
	}
}
