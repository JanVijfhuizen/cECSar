#pragma once
#include <cstdint>

namespace jecs
{
	struct Entity final
	{
		class Hasher final
		{
		public:
			size_t operator()(const Entity& entity) const;
		};

		int32_t index = -1;
		int32_t id = -1;

		bool operator ==(const Entity& other) const;
		bool operator !=(const Entity& other) const;
	};
}
