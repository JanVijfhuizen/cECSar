#pragma once

namespace cecsar
{
	class Cecsar;
	struct EntityInfo;

	class IEntityFactory
	{
		friend Cecsar;

	public:
		virtual ~IEntityFactory();

	protected:
		virtual void Initialize(Cecsar& cecsar);
		virtual void Construct(Cecsar& cecsar, const EntityInfo& info);
	};
}
