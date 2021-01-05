#pragma once
#include <vector>

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
		std::vector<T*> _inactive;
	};

	template <typename T>
	Pool<T>::~Pool()
	{
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

		return *instance;
	}

	template <typename T>
	constexpr void Pool<T>::Push(T& instance)
	{
		_inactive.push_back(&instance);
	}
}
