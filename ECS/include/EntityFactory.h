#pragma once
#include <IEntityFactory.h>
#include <Cecsar.h>

namespace cecsar
{
	/*
	OPTIONAL.
	Inherit from the EntityFactory to predefine entity construction.

	EXAMPLE:
	A MarioFactory would add a Renderer, Transform, Collider and MarioBehaviour for instance,
	and set variables like renderer.texture to streamline construction.
	*/
	class EntityFactory : public IEntityFactory
	{
	public:
		/*
		Used to construct a set of components over an entity.
		DOESN'T CREATE AN ENTITY HOWEVER!
		*/
		void Construct(Cecsar& cecsar, const EntityInfo& info) final override;

	protected:
		/*
		Override to define custom construction behaviour.
		*/
		virtual void OnConstruction(Cecsar& cecsar, const EntityInfo& info) = 0;
	};

	inline void EntityFactory::Construct(Cecsar& cecsar, const EntityInfo& info)
	{
		OnConstruction(cecsar, info);
	}
}
