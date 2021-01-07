#pragma once
#include <Factories/StandardFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class GroundBlockFactory final : public StandardFactory
	{
	protected:
		inline FactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
		FactoryImp<Collider>* SetColliderImp(cecsar::Cecsar& cecsar) override;
	};

	inline FactoryImp<Renderer>* GroundBlockFactory::SetRenderImp(cecsar::Cecsar& cecsar)
	{
		const auto renderer = new StandardRendererImp;
		renderer->path = "Art/GroundBlock.png";
		return renderer;
	}

	inline FactoryImp<Collider>* GroundBlockFactory::SetColliderImp(cecsar::Cecsar& cecsar)
	{
		const auto collider = new FactoryImp<Collider>;
		collider->prototype.targetMask = 0;
		return collider;
	}
}
