#pragma once

namespace cecsar
{
	class Cecsar;

	class IModule
	{
		friend Cecsar;

	public:
		virtual ~IModule();

	private:
		virtual void Initialize(Cecsar& cecsar);
	};
}
