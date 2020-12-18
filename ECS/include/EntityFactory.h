#pragma once
#include <IEntityFactory.h>
#include <cecsar.h>

namespace cecsar
{
	class EntityFactory : public IEntityFactory
	{
	protected:
		void Construct(Cecsar& cecsar, int32_t index) final override;
		virtual void OnConstruction(Cecsar& cecsar, int32_t index) = 0;
	};

	inline void EntityFactory::Construct(Cecsar& cecsar, const int32_t index)
	{
		OnConstruction(cecsar, index);
	}
}
