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

		constexpr JobConverterModule& GetJobConvModule() const;
		constexpr JobSystemModule& GetJobModule() const;

	private:
		JobSystemModule* _jobModule = nullptr;
		JobConverterModule* _jobConverter = nullptr;
	};

	template <typename ... Args>
	void JobSystem<Args...>::Initialize(cecsar::Cecsar& cecsar)
	{
		_jobModule = &cecsar.GetModule<JobSystemModule>();
		_jobConverter = &cecsar.GetModule<JobConverterModule>();
	}

	template <typename ... Args>
	constexpr JobConverterModule& JobSystem<Args...>::GetJobConvModule() const
	{
		return *_jobConverter;
	}

	template <typename ... Args>
	constexpr JobSystemModule& JobSystem<Args...>::GetJobModule() const
	{
		return *_jobModule;
	}
}
