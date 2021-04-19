#pragma once
#include <vector>
#include <unordered_set>
#include <queue>

#include "Entity.h"

namespace jecs
{
	class Cecsar final
	{
	public:
		int32_t globalIndex = 0;

		Cecsar();
		~Cecsar();

		static Cecsar& Get();

		Entity Spawn();
		void EraseAt(int32_t index);

		bool Contains(int32_t index);
		Entity Get(int32_t index);
		bool Validate(Entity entity);

		void PushDependency(void* dependency);

	private:
		static Cecsar* _instance;

		std::unordered_set<Entity, Entity::Hasher> _entities{};
		std::priority_queue<int32_t, std::vector<int32_t>, std::greater<>> _open{};

		std::vector<void*> _dependencies{};
	};
}
