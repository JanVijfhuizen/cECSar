#pragma once
#include "IModule.h"
#include "Cecsar.h"
#include "JobSystemModule.h"

namespace game
{
	/*
	I didn't want to make game related code into a dependency for the job system, 
	so that's why I created a second class to house the game specific code.
	*/
	class JobConverterModule final : public cecsar::IModule
	{
	public:
		typedef std::function<void(int32_t start, int32_t stop)> LinearJob;

		inline void ToLinearJobs(int32_t count, const LinearJob& job) const;

		inline void Start() const;
		inline void Wait() const;

	protected:
		inline void Initialize(cecsar::Cecsar& cecsar) override;

	private:
		JobSystemModule* _jobSystem = nullptr;
	};

	inline void JobConverterModule::ToLinearJobs(const int32_t count, const LinearJob& job) const
	{
		const int32_t threadCount = _jobSystem->GetThreadCount();
		const int32_t chunk = count / threadCount;

		for (int32_t i = 0; i < threadCount; ++i)
		{
			const int32_t start = i * chunk;
			const int32_t stop = start + chunk;

			if (stop <= start)
				break;

			_jobSystem->Enqueue([start, stop, job]() 
			{
				job(start, stop);
			});
		}

		{
			const int32_t remainder = count - chunk * threadCount;
			const int32_t start = count - remainder;
			const int32_t stop = count;

			if (stop <= start)
				return;

			_jobSystem->Enqueue([start, stop, job]()
			{
				job(start, stop);
			});
		}
	}

	inline void JobConverterModule::Start() const
	{
		_jobSystem->Start();
	}

	inline void JobConverterModule::Wait() const
	{
		_jobSystem->Wait();
	}

	inline void JobConverterModule::Initialize(cecsar::Cecsar& cecsar)
	{
		_jobSystem = &cecsar.GetModule<JobSystemModule>();
	}
}
