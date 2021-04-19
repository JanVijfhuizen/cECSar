#pragma once
#include "Cecsar.h"
#include "Observer.h"

namespace jecs
{
	template <typename Child>
	class Set : public Observer
	{
	public:
		virtual void EraseAt(int32_t index) = 0;

		static Child& Get();

	protected:
		Set();

		void OnErase(Entity entity) override;

	private:
		static Child* _instance;
	};

	template <typename Child>
	Child& Set<Child>::Get()
	{
		if (!_instance)
			_instance = new Child();
		return *_instance;
	}

	template <typename Child>
	Set<Child>::Set() : Observer()
	{
		delete _instance;
		_instance = static_cast<Child*>(this);

		Cecsar::Get().PushDependency(this);
	}

	template <typename Child>
	void Set<Child>::OnErase(const Entity entity)
	{
		EraseAt(entity.index);
	}

	template <typename Child>
	Child* Set<Child>::_instance = nullptr;
}
