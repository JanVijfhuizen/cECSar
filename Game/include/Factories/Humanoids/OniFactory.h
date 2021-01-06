#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class OniFactory final : public HumanoidFactory
	{
	protected:
		inline IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
		inline IFactoryImp<RigidBody>* SetRigidBodyImp(const cecsar::Cecsar& cecsar) override;
		inline IFactoryImp<Collider>* SetColliderImp(cecsar::Cecsar& cecsar) override;
	};

	inline IFactoryImp<Renderer>* OniFactory::SetRenderImp(cecsar::Cecsar& cecsar)
	{
		const auto renderer = new StandardRendererImp;
		renderer->path = "Art/Oni.png";
		renderer->prototype.count = 6;
		return renderer;
	}

	inline IFactoryImp<RigidBody>* OniFactory::SetRigidBodyImp(const cecsar::Cecsar& cecsar)
	{
		const auto rigidBody = new IFactoryImp<RigidBody>;
		rigidBody->prototype.weight = 1.5f;
		return rigidBody;
	}

	inline IFactoryImp<Collider>* OniFactory::SetColliderImp(cecsar::Cecsar& cecsar)
	{
		const auto collider = new IFactoryImp<Collider>;
		//collider->prototype.circle.radius *= 2;
		return collider;
	}
}
