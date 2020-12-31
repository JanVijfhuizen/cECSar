#pragma once
#include <IComponentSystem.h>
#include <Cecsar.h>

namespace cecsar
{
	/*
	OPTIONAL.
	Inherit from this class to be able to use it with cecsar.

	ComponentSystems are used to define the behaviour for one or more
	components.

	EXAMPLE:
	A Transform component would have their corresponding TransformSystem<Transform>,
	and a renderer that also might need the Transform would be a RendererSystem<Renderer, Transform>.
	*/
	template <typename ...Args>
	class ComponentSystem : public IComponentSystem
	{
	private:
		/*
		Called from cecsar.Update<ThisComponentSystem>.
		*/
		void Update(Cecsar& cescar) final override;
		/*
		Override this to define custom behaviour.
		*/
		virtual void OnUpdate(utils::SparseSet<Args>&...) = 0;
	};

	template <typename ... Args>
	void ComponentSystem<Args...>::Update(Cecsar& cescar)
	{
		OnUpdate(cescar.GetSet<Args>()...);
	}
}
