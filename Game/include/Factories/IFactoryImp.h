#pragma once
#include <Cecsar.h>

namespace game
{
	class IFactoryImp
	{
	public:
		inline virtual ~IFactoryImp();

		virtual void Initialize(cecsar::Cecsar& cecsar) = 0;
		virtual void Construct(cecsar::Cecsar& cecsar, int32_t index) = 0;
	};

	template <typename T>
	class FactoryImp : public IFactoryImp
	{
	public:
		T prototype;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void Construct(cecsar::Cecsar& cecsar, int32_t index) final override;
		
		virtual void OnConstruction(
			cecsar::Cecsar& cecsar, T& component, int32_t index);
	};

	inline IFactoryImp::~IFactoryImp() = default;

	template <typename T>
	void FactoryImp<T>::Initialize(cecsar::Cecsar&)
	{

	}

	template <typename T>
	void FactoryImp<T>::Construct(cecsar::Cecsar& cecsar, const int32_t index)
	{
		auto& component = cecsar.AddComponent<T>(index);
		OnConstruction(cecsar, component, index);
	}

	template <typename T>
	void FactoryImp<T>::OnConstruction(cecsar::Cecsar&, T& component, const int32_t)
	{
		component = prototype;
	}
}