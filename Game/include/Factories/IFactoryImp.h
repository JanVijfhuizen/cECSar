#pragma once
#include <Cecsar.h>

namespace game
{
	template <typename T>
	class IFactoryImp
	{
	public:
		T blueprint;

		virtual ~IFactoryImp();

		virtual void Initialize(cecsar::Cecsar& cecsar);
		virtual void OnConstruction(
			cecsar::Cecsar& cecsar, T& component, int32_t index);
	};

	template <typename T>
	IFactoryImp<T>::~IFactoryImp() = default;

	template <typename T>
	void IFactoryImp<T>::Initialize(cecsar::Cecsar&)
	{

	}

	template <typename T>
	void IFactoryImp<T>::OnConstruction(cecsar::Cecsar&, T& component, const int32_t)
	{
		component = blueprint;
	}
}