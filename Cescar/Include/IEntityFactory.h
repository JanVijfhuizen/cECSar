#pragma once
#include <cstdint>

namespace cecsar
{
	class Cecsar;

	class IEntityFactory
	{
		friend Cecsar;

	public:
		virtual ~IEntityFactory();

	protected:
		virtual void Initialize(Cecsar& cecsar);
		virtual void Construct(Cecsar& cecsar, int32_t index) = 0;
	};
}
