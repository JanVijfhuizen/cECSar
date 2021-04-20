#pragma once
#include <vector>
#include <unordered_set>
#include <queue>

#include "Entity.h"

namespace jecs
{
	class Dependency
	{
	public:
		virtual ~Dependency();

		virtual void Preload();
		virtual void Load();
		virtual void Save();
	};

	// A lightweight ECS framework that is designed to be extendable.
	class Cecsar final
	{
	public:
		struct LoadInfo final
		{
			// Load set data from memory.
			bool loadFromFile = false;
			std::string loadPostfix = "";
		};

		explicit Cecsar(int32_t capacity = 1e4);
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
		void PushDependency(Dependency* dependency);

		// Get information about the currently spawned cecsar.
		[[nodiscard]] bool GetCecsarLoaded() const;
		[[nodiscard]] std::string GetPostfix() const;
		[[nodiscard]] int32_t GetDefaultCapacity() const;

		// Load entities from disk.
		bool TryLoad(const std::string& postfix);
		// Save entities to disk.
		void Save(const std::string& postfix);

	private:
		static Cecsar* _instance;

		int32_t _globalId = 0;
		int32_t _defaultCapacity = 1e4;
		bool _loaded = false;
		std::string _postfix = "";

		std::unordered_set<Entity, Entity::Hasher> _entities{};
		std::priority_queue<int32_t, std::vector<int32_t>, std::greater<>> _openPq{};
		std::unordered_set<int32_t> _openSet; 

		std::vector<Dependency*> _dependencies{};
	};
}
