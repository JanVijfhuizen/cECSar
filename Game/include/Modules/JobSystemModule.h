#pragma once
#include "IModule.h"
#include <queue>
#include <functional>
#include <thread>
#include <mutex>

namespace game
{
	constexpr int32_t THREAD_NUM = 6;

	class JobSystemModule final : public cecsar::IModule
	{
	public:
		typedef std::function<void()> Job;

		~JobSystemModule();
		inline void Initialize(cecsar::Cecsar& cecsar) override;

		inline void Enqueue(const Job& job);
		inline bool IsDone();
		inline void Wait();

		inline int32_t GetThreadCount() const;

	private:
		std::queue<Job> _jobPool{};
		std::vector<std::thread> _threads{};

		std::mutex _mutexJobRequest{};
		std::condition_variable _cvJobRequest{};

		std::mutex _mutexJobPool{};
		int32_t _activeNum = 0, _inactiveNum = 0;
		int32_t _threadNum = 0;

		inline bool RequestJob(Job& job);
	};

	inline JobSystemModule::~JobSystemModule()
	{
		for (auto& thread : _threads)
			thread.detach();
	}

	inline void JobSystemModule::Initialize(cecsar::Cecsar& cecsar)
	{
		_threadNum = std::thread::hardware_concurrency();
		for (int32_t i = _threadNum - 1; i >= 0; --i)
			_threads.emplace_back([this]()
				{
					while (true)
					{
						Job job;

						if (RequestJob(job))
						{
							job();

							{
								std::unique_lock<std::mutex> lk(_mutexJobPool);
								_activeNum--;
							}
							continue;
						}

						std::unique_lock<std::mutex> lock(_mutexJobRequest);
						_cvJobRequest.wait(lock);
					}
				});
	}

	inline void JobSystemModule::Enqueue(const Job& job)
	{
		std::unique_lock<std::mutex> lk(_mutexJobPool);

		_jobPool.push(job);
		_inactiveNum++;

		_cvJobRequest.notify_one();
	}

	inline bool JobSystemModule::IsDone()
	{
		std::unique_lock<std::mutex> lk(_mutexJobPool);
		return _activeNum + _inactiveNum == 0;
	}

	inline void JobSystemModule::Wait()
	{
		while (!IsDone())
			std::this_thread::yield();
	}

	inline int32_t JobSystemModule::GetThreadCount() const
	{
		return _threadNum;
	}

	inline bool JobSystemModule::RequestJob(Job& job)
	{
		std::unique_lock<std::mutex> lk(_mutexJobPool);
		if (_jobPool.empty())
			return false;

		job = _jobPool.front();
		_jobPool.pop();

		_activeNum++;
		_inactiveNum--;
		return true;
	}
}
