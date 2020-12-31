#pragma once

namespace cecsar
{
	class Cecsar;

	/*
	Used to avoid circular dependencies.
	Use ComponentSystem instead.
	*/
	class IComponentSystem
	{
		friend Cecsar;

	public:
		virtual ~IComponentSystem();

	private:
		virtual inline void Update(Cecsar& cescar) = 0;
		virtual void Initialize(Cecsar& cecsar);
	};
}