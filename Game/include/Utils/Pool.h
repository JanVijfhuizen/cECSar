#pragma once
#include <queue>

namespace utils
{
	template <typename T>
	class Pool final
	{
	public:
		~Pool();

		constexpr T& Get();
		constexpr void Push(T& instance);

	private:
		std::vector<T*> _active, _inactive;
	};

	template <typename T>
	Pool<T>::~Pool()
	{
		for (auto active : _active)
			delete active;
		for (auto inactive : _inactive)
			delete inactive;
	}

	template <typename T>
	constexpr T& Pool<T>::Get()
	{
		T* instance;

		if (_inactive.empty())
			instance = new T;
		else
		{
			instance = _inactive.back();
			_inactive.pop_back();
		}

		_active.push_back(instance);
		return *instance;
	}

	template <typename T>
	constexpr void Pool<T>::Push(T& instance)
	{
		auto position = std::find(_active.begin(), _active.end(), &instance);
		_active.erase(position);
		_inactive.push_back(&instance);
	}
}
