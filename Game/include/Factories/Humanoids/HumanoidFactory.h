#pragma once
#include <Factories/StandardFactory.h>
#include "HandFactory.h"
#include "HeadFactory.h"

namespace game
{
	class HumanoidFactory : public StandardFactory
	{
	protected:
		void OnInitializeCustom(cecsar::Cecsar& cecsar) override;
		void OnConstructionCustom(
			cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info) override;

		template <typename T>
		void SetHandFactoryImpl(cecsar::Cecsar& cecsar);

		template <typename T>
		void SetHeadFactoryImpl(cecsar::Cecsar& cecsar);

	private:
		HandFactory* _handFactory = nullptr;
		HeadFactory* _headFactory = nullptr;
	};

	template <typename T>
	void HumanoidFactory::SetHandFactoryImpl(cecsar::Cecsar& cecsar)
	{
		_handFactory = static_cast<HandFactory*>(&cecsar.GetFactory<T>());
	}

	template <typename T>
	void HumanoidFactory::SetHeadFactoryImpl(cecsar::Cecsar& cecsar)
	{
		_headFactory = static_cast<HeadFactory*>(&cecsar.GetFactory<T>());
	}
}
