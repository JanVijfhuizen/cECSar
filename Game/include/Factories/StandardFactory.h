#pragma once
#include "EntityFactory.h"
#include "IFactoryImp.h"
#include <map>

namespace game
{
	class StandardFactory : public cecsar::EntityFactory
	{
	public:
		~StandardFactory() override;

	protected:
		void Initialize(cecsar::Cecsar& cecsar) final override;

		virtual void OnInitialize(cecsar::Cecsar& cecsar) = 0;
		void OnConstruction(
			cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info) override;

		template <typename Component, typename Imp>
		Imp& DefineImplementation();

	private:
		cecsar::Cecsar* _cecsar = nullptr;
		std::map<std::type_index, IFactoryImp*> _imps;

		template <typename Component, typename Imp>
		Imp& SetImplementation();
	};

	template <typename Component, typename Imp>
	Imp& StandardFactory::DefineImplementation()
	{
		if (_imps.count(typeid(Component)) == 0)
			return SetImplementation<Component, Imp>();

		auto imp = _imps[typeid(Component)];
		auto cast = dynamic_cast<Imp*>(imp);
		if(!cast)
		{
			delete imp;
			return SetImplementation<Component, Imp>();
		}

		return *static_cast<Imp*>(imp);
	}

	template <typename Component, typename Imp>
	Imp& StandardFactory::SetImplementation()
	{
		auto imp = new Imp();
		imp->Initialize(_cecsar);
		_imps[typeid(Component)] = imp;
		return static_cast<Imp*>(imp);
	}
}
