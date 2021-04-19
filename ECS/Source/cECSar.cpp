#include "pch.h"
#include "cECSar.h"
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

	void Cecsar::EraseAt(const int32_t index)
	{
		const Entity empty;

		_entities.erase(empty);
		_open.emplace(index);
	}

	bool Cecsar::Contains(const int32_t index)
	{
		const Entity key
		{
			index
		};

		return _entities.find(key) != _entities.end();
	}

	Entity Cecsar::Get(const int32_t index)
	{
		const Entity key
		{
			index
		};

		const auto it = _entities.find(key);
		assert(it != _entities.end());
		return *it;
	}

	bool Cecsar::Validate(const Entity entity)
	{
		const auto it = _entities.find(entity);
		if (it == _entities.end())
			return false;
		return entity == *it;
	}

	void Cecsar::PushDependency(void* dependency)
	{
		_dependencies.push_back(dependency);
	}
}
