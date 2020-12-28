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
		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info) override;

		virtual IFactoryImp<Renderer>* SetRenderImp(cecsar::Cecsar& cecsar);
		virtual IFactoryImp<Transform>* SetTransformImp(cecsar::Cecsar& cecsar);

	private:
		IFactoryImp<Renderer>* _renderImp = nullptr;
		IFactoryImp<Transform>* _transformImp = nullptr;
	};
}
