#pragma once

namespace cecsar
{
	class Cecsar;
	struct EntityInfo;

	/*
	Used to avoid circular dependencies.
	Use EntityFactory instead.
	*/
	class IEntityFactory
	{
		friend Cecsar;

	public:
		virtual ~IEntityFactory();
		virtual void Construct(Cecsar& cecsar, const EntityInfo& info);

	protected:
		virtual void Initialize(Cecsar& cecsar);
	};
}
