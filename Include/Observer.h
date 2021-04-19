#pragma once
#include <vector>

namespace jecs
{
	class Cecsar;
	struct Entity;

	// Inherit from this to observe cecsar related events.
	class Observer
	{
		friend Cecsar;

	protected:
		Observer();

	private:
		virtual void OnSpawn(Entity entity);
		virtual void OnErase(Entity entity);

		static std::vector<Observer*> _observers;
	};
}
