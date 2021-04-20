#pragma once
#include <vector>
#include <unordered_set>
#include <queue>

#include "Entity.h"

namespace jecs
{
	// A lightweight ECS framework that is designed to be extendable.
	class Cecsar final
	{
	public:
		// If an entity is spawned, the ID is assigned to the global index,
		// After which this increments.
		int32_t globalIndex = 0;

		// If a set's size is not explicitely defined, it will spawn with this capacity.
		int32_t setDefaultCapacity = 1e4;

		// Load set data from memory.
		bool loadFromFile = false;
		std::string loadPostfix = "";

		Cecsar();
		~Cecsar();

		// Lazy singleton getter.
		static Cecsar& Get();

		// Spawn an entity.
		Entity Spawn();
		// Erase an entity at target index.
		void Erase(Entity entity);
		// Check if an entity is still alive.
		bool IsAlive(Entity entity);

		[[nodiscard]] int32_t GetCount() const;

		// If you want something to be destroyed when cecsar goes out of scope,
		// Add it in here.
		// Everything that inherits from Module will automatically do so.
		void PushDependency(void* dependency);

	private:
		static Cecsar* _instance;

		std::unordered_set<Entity, Entity::Hasher> _entities{};
		std::priority_queue<int32_t, std::vector<int32_t>, std::greater<>> _open{};

		std::vector<void*> _dependencies{};
	};
}
