#pragma once
#include "cECSar.h"

namespace jecs
{
	template <typename T>
	class Module
	{
		friend Cecsar;

	public:
		static T& Get();

	protected:
		Module();
		virtual ~Module();

	private:
		static T* _instance = nullptr;
	};

	template <typename T>
	Module<T>::Module()
	{
		delete _instance;
		_instance = this;

		Cecsar::Get().PushDependency(this);
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
			_instance = new T();
		return *_instance;
	}
}