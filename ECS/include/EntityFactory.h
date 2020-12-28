#pragma once
#include <IEntityFactory.h>
#include <Cecsar.h>

namespace cecsar
{
	class EntityFactory : public IEntityFactory
	{
	protected:
		void Construct(Cecsar& cecsar, const EntityInfo& info) final override;
		virtual void OnConstruction(Cecsar& cecsar, const EntityInfo& info) = 0;
	};

	inline void EntityFactory::Construct(Cecsar& cecsar, const EntityInfo& info)
	{
		OnConstruction(cecsar, info);
	}
}
