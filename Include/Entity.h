#pragma once
#include <cstdint>

namespace jecs
{
	// This serves as an object in the world.
	// You can add components to an entity to define it's behaviour.
	struct Entity final
	{
		class Hasher final
		{
		public:
			size_t operator()(const Entity& entity) const;
		};

		explicit Entity(int32_t index = -1, int32_t id = -1);

		// Used as a key to get components from a set.
		int32_t index = -1;
		// Used to compare entities.
		int32_t id = -1;

		bool operator ==(const Entity& other) const;
		bool operator !=(const Entity& other) const;
	};
}
