#include "pch.h"
#include "Cecsar.h"
#include "Observer.h"
#include <cassert>
#include <fstream>

namespace jecs
{
	Cecsar* Cecsar::_instance = nullptr;

	Dependency::~Dependency() = default;

	void Dependency::Preload()
	{
	}

	void Dependency::Load()
	{
	}

	void Dependency::Save()
	{
	}

	Cecsar::Cecsar(const int32_t capacity) : _defaultCapacity(capacity)
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
		int32_t index = _entities.size();
		if (!_openPq.empty())
		{
			index = _openPq.top();
			_openPq.pop();
			_openSet.erase(index);
		}

		const Entity entity
		{
			index,
			_globalId++
		};

		_entities.insert(entity);
		for (auto& observer : Observer::_observers)
			observer->OnSpawn(entity);

		return entity;
	}

	void Cecsar::Erase(const Entity entity)
	{
		_entities.erase(entity);
		_openPq.emplace(entity.index);
		_openSet.insert(entity.index);

		for (auto& observer : Observer::_observers)
			observer->OnErase(entity);
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

	void Cecsar::PushDependency(Dependency* dependency)
	{
		_dependencies.push_back(dependency);
		if (_loaded)
			dependency->Load();
	}

	bool Cecsar::GetCecsarLoaded() const
	{
		return _loaded;
	}

	std::string Cecsar::GetPostfix() const
	{
		return _postfix;
	}

	int32_t Cecsar::GetDefaultCapacity() const
	{
		return _defaultCapacity;
	}

	bool Cecsar::TryLoad(const std::string& postfix)
	{
		_entities.clear();
		_openPq = {};
		_openSet.clear();

		std::ifstream file;
		file.open("Cecsar" + postfix, std::ios::in);
		_loaded = file.good();
		if (!_loaded)
			return false;

		file.read(reinterpret_cast<char*>(&_globalId), sizeof(int32_t));
		while (!file.eof())
		{
			Entity entity;
			file.read(reinterpret_cast<char*>(&entity), sizeof(Entity));
			_entities.insert(entity);
		}

		std::ofstream file2;
		file.open("Cecsar2" + postfix, std::ios::out);
		while (!file.eof())
		{
			int32_t index;
			file.read(reinterpret_cast<char*>(&index), sizeof(int32_t));

			_openPq.emplace(index);
			_openSet.insert(index);
		}

		for (auto& dependency : _dependencies)
			dependency->Preload();
		for (auto& dependency : _dependencies)
			dependency->Load();

		return true;
	}

	void Cecsar::Save(const std::string& postfix)
	{
		std::ofstream file;
		file.open("Cecsar" + postfix, std::ios::out);

		file.write(reinterpret_cast<char*>(&_globalId), sizeof(int32_t));
		for (auto& entity : _entities)
		{
			Entity writeable = entity;
			file.write(reinterpret_cast<char*>(&writeable), sizeof(Entity));
		}

		std::ofstream file2;
		file.open("Cecsar2" + postfix, std::ios::out);
		for (auto& index : _openSet)
		{
			int32_t writeable = index;
			file.write(reinterpret_cast<char*>(&writeable), sizeof(int32_t));
		}

		for (auto& dependency : _dependencies)
			dependency->Save();
	}
}
