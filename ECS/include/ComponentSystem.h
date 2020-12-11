#pragma once
#include <IComponentSystem.h>
#include <Cecsar.h>

namespace cecsar
{
	template <typename ...Args>
	class ComponentSystem : public IComponentSystem
	{
	private:
		void Update(Cecsar& cescar) final override;
		virtual void OnUpdate(utils::SparseSet<Args>&...) = 0;
	};

	template <typename ... Args>
	void ComponentSystem<Args...>::Update(Cecsar& cescar)
	{
		OnUpdate(cescar.GetSet<Args>()...);
	}
}
