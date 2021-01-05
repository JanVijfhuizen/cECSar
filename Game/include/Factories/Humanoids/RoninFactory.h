#pragma once
#include <Factories/Humanoids/HumanoidFactory.h>
#include <Factories/Implementations/StandardRendererImp.h>

namespace game
{
	class RoninFactory final : public HumanoidFactory
	{
	public:
		inline ~RoninFactory() override;

	protected:
		IFactoryImp<CameraFollowTarget>* _followTargetImp = nullptr;

		inline void Initialize(cecsar::Cecsar& cecsar) override;
		inline void OnConstruction(cecsar::Cecsar& cecsar, 
			const cecsar::EntityInfo& info) override;

		inline IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar) override;
		inline IFactoryImp<Controller>* SetControllerImp(cecsar::Cecsar& cecsar) override;

		inline IFactoryImp<CameraFollowTarget>* SetFollowTargetImp(cecsar::Cecsar& cecsar);
	};

	inline RoninFactory::~RoninFactory()
	{
		delete _followTargetImp;
	}

	inline void RoninFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		HumanoidFactory::Initialize(cecsar);
		_followTargetImp = SetFollowTargetImp(cecsar);
	}

	inline void RoninFactory::OnConstruction(cecsar::Cecsar& cecsar, 
		const cecsar::EntityInfo& info)
	{
		HumanoidFactory::OnConstruction(cecsar, info);

		auto& followTarget = cecsar.AddComponent<CameraFollowTarget>(info.index);
		_followTargetImp->OnConstruction(cecsar, followTarget, info.index);
	}

	inline IFactoryImp<Renderer>* RoninFactory::SetRenderImp(
		cecsar::Cecsar& cecsar)
	{
		auto renderer = new StandardRendererImp;
		renderer->path = "Art/Ronin.png";
		renderer->prototype.count = 6;
		return renderer;
	}

	inline IFactoryImp<Controller>* RoninFactory::SetControllerImp(
		cecsar::Cecsar& cecsar)
	{
		auto controller = new IFactoryImp<Controller>;
		controller->prototype.type = ControllerType::player;
		return controller;
	}

	inline IFactoryImp<CameraFollowTarget>* RoninFactory::SetFollowTargetImp(
		cecsar::Cecsar& cecsar)
	{
		return new IFactoryImp<CameraFollowTarget>;
	}
}
