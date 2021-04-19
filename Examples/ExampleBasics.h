#pragma once

#include "Cecsar.h"
#include "Module.h"
#include "SparseSet.h"

namespace jecs::example
{
	// Some component.
	struct Transform
	{
		float x;
		float y;
		float z;
	};

	// Some module.
	class SomeSystem : public jecs::Module<SomeSystem>
	{
	public:
		void Update()
		{
			// The component sets are singletons, meaning you can get if from anywhere.
			auto& transforms = jecs::SparseSet<Transform>::Get();

			float yHighestValue = 0;
			int yHighestIndex = 0;

			// This is just some random usecase.
			// Here we're looking for the highest transform.
			for (auto [instance, index] : transforms)
			{
				if (instance.x > yHighestValue)
				{
					yHighestIndex = index;
					yHighestValue = instance.x;
				}
			}

			// Get a specific transform by index.
			Transform& highest = transforms[yHighestIndex];
			// Do some stuff with this transform.
		}
	};

	int RunExampleBasics()
	{
		{
			// If this goes out of scope, every module that has been created will also be deleted.
			jecs::Cecsar cecsar;

			// Every module of cecsar is a singleton.
			// This is a lazy getter, which means that it checks whether or not an instance exists,
			// and if not, it creates a new one and returns that. 
			// Otherwise it returns the already existing instance.
			SomeSystem& system = SomeSystem::Get();

		} // Cecsar and every dependency will now be deleted.

		jecs::Cecsar cecsar;

		// Spawn an entity.
		jecs::Entity entity1 = cecsar.Spawn();
		jecs::Entity entity2 = cecsar.Spawn();

		// Compare entities.
		if (entity1 == entity2)
		{
			// Some unreachable code
		}

		// Check if an entity is still alive.
		cecsar.IsAlive(entity1);

		// Add a component to the entity.
		// Use it's index to create a new transform.
		auto& transforms = jecs::SparseSet<Transform>::Get();
		transforms.Insert(entity1.index);

		// Get a module and do something with it.
		SomeSystem& system = SomeSystem::Get();
		system.Update();

		// Now remove the component.
		transforms.EraseAt(entity1.index);
		// And delete the entity.
		// All the attached components will be deleted automatically.
		cecsar.Erase(entity1);

		return 0;
	}
}