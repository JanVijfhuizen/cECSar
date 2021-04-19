#include "pch.h"
#include "Observer.h"
#include "Entity.h"

namespace jecs
{
	std::vector<Observer*> Observer::_observers = {};

	Observer::Observer()
	{
		_observers.push_back(this);
	}

	void Observer::OnSpawn(const Entity entity)
	{
	}

	void Observer::OnErase(const Entity entity)
	{
	}
}
