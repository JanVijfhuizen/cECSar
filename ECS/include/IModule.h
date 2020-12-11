#pragma once

namespace cecsar
{
	class Cecsar;

	class IModule
	{
		friend Cecsar;

	public:
		virtual ~IModule();

	protected:
		virtual void Initialize(Cecsar& cecsar);
	};
}
