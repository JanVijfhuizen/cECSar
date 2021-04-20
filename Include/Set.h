#pragma once
#include "Cecsar.h"
#include "Observer.h"
#include <vcruntime_typeinfo.h>

namespace jecs
{
	// Inherit from this to define your own set.
	// Usually the sparse set is sufficient, but there might come a time when you
	// need something more specialized.
	template <typename Child>
	class Set : public Dependency, public Observer
	{
	public:
		virtual void EraseAt(int32_t index) = 0;

		static Child& Get();

		template <typename T>
		static std::string GetFilePath();

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
	template <typename T>
	std::string Set<Child>::GetFilePath()
	{
		const std::string post{ Cecsar::Get().GetPostfix() + ".txt" };
		const std::string pre{ std::string(typeid(T).name()) };
		const std::string final{ pre.substr(pre.find_last_of(':') + 1) + post };
		return final;
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
