#pragma once
#include <IEntityFactory.h>
#include <Cecsar.h>

namespace cecsar
{
	template <typename ... Args>
	class EntityFactory : public IEntityFactory
	{
	protected:
		void Construct(Cecsar& cecsar, int32_t index) final override;
		virtual void OnConstruction(int32_t index, Args&... args) = 0;
	};

	template <typename ... Args>
	void EntityFactory<Args...>::Construct(Cecsar& cecsar, int32_t index)
	{
		OnConstruction(index, cecsar.GetSet<Args>().Insert(index)...);
	}
}
