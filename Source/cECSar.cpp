#include "pch.h"
#include "Cecsar.h"
#include "Observer.h"
#include <cassert>

namespace jecs
{
	Cecsar* Cecsar::_instance = nullptr;

	Cecsar::Cecsar()
	{
		delete _instance;
		_instance = this;
	}

	Cecsar::~Cecsar()
	{
		for (auto module : _dependencies)
			delete module;
		Observer::_observers.clear();
		_instance = nullptr;
	}

	Cecsar& Cecsar::Get()
	{
		assert(_instance);
		return *_instance;
	}

	Entity Cecsar::Spawn()
	{
		int32_t index = -1;
		if (!_open.empty())
		{
			index = _open.top();
			_open.pop();
		}

		const Entity entity
		{
			index,
			globalIndex++
		};

		_entities.insert(entity);
		return entity;
	}

	void Cecsar::Erase(const Entity entity)
	{
		_entities.erase(entity);
		_open.emplace(entity.index);
	}

	bool Cecsar::IsAlive(const Entity entity)
	{
		const auto it = _entities.find(entity);
		if (it == _entities.end())
			return false;
		return entity == *it;
	}

	int32_t Cecsar::GetCount() const
	{
		return _entities.size();
	}

	void Cecsar::PushDependency(void* dependency)
	{
		_dependencies.push_back(dependency);
	}
}
