#pragma once
#include "EntityFactory.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "IFactoryImp.h"

namespace game
{
	class StandardFactory : public cecsar::EntityFactory
	{
	public:
		~StandardFactory() override;

	protected:
		IFactoryImp<Renderer>* _renderImp = nullptr;
		IFactoryImp<Transform>* _transformImp = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(cecsar::Cecsar& cecsar, int32_t index) override;

		virtual IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar);
		virtual IFactoryImp<Transform>* SetTransformImp(cecsar::Cecsar& cecsar);
	};
}
