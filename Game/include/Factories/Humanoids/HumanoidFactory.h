#pragma once
#include <Factories/StandardFactory.h>
#include "Components/MovementComponent.h"
#include "Components/Controller.h"

namespace game
{
	class HumanoidFactory : public StandardFactory
	{
	public:
		~HumanoidFactory() override;

	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info) override;

		virtual IFactoryImp<MovementComponent>* SetMovementImp(cecsar::Cecsar& cecsar);
		virtual IFactoryImp<Controller>* SetControllerImp(cecsar::Cecsar& cecsar);

	private:
		IFactoryImp<MovementComponent>* _movementImp = nullptr;
		IFactoryImp<Controller>* _controllerImp = nullptr;
	};
}
