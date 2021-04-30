#pragma once
#include "Cecsar.h"

namespace jecs
{
	// Inherit from this to automatically make your class a cecsar-bound singleton.
	template <typename T>
	class Module : public Dependency
	{
		friend Cecsar;

	public:
		static T& Get();

	protected:
		Module();
		~Module();

	private:
		static T* _instance;
	};

	template <typename T>
	Module<T>::Module()
	{
		delete _instance;
		_instance = static_cast<T*>(this);
	}

	template <typename T>
	Module<T>::~Module()
	{
		if (_instance == this)
			_instance = nullptr;
	}

	template <typename T>
	T& Module<T>::Get()
	{
		if (!_instance)
			new T();
		return *_instance;
	}

	template <typename T>
	T* Module<T>::_instance = nullptr;
}