#pragma once
#include "ComponentSystem.h"
#include "Modules/JobConverterModule.h"

namespace game
{
	template <typename ...Args>
	class JobSystem : public cecsar::ComponentSystem<Args...>
	{
	protected:
		void Initialize(cecsar::Cecsar& cecsar) override;
		JobConverterModule& GetJobModule() const;

	private:
		JobConverterModule* _jobConverter = nullptr;
	};

	template <typename ... Args>
	void JobSystem<Args...>::Initialize(cecsar::Cecsar& cecsar)
	{
		_jobConverter = &cecsar.GetModule<JobConverterModule>();
	}

	template <typename ... Args>
	JobConverterModule& JobSystem<Args...>::GetJobModule() const
	{
		return *_jobConverter;
	}
}
