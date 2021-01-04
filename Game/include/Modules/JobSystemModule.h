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

		inline ~JobSystemModule();
		inline void Initialize(cecsar::Cecsar& cecsar) override;

		inline void Enqueue(const Job& job);

		inline void Start();
		inline void Wait();

		inline bool IsDone();

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
		_threadNum = std::max(1, 
			static_cast<int32_t>(std::thread::hardware_concurrency()) - 1);
		for (int32_t i = _threadNum - 1; i >= 0; --i)
			_threads.emplace_back(std::thread([this]()
				{
					while (true)
					{
						Job job;

						while (RequestJob(job))
						{
							job();

							std::unique_lock<std::mutex> lk(_mutexJobPool);
							_activeNum--;
						}

						std::unique_lock<std::mutex> lock(_mutexJobRequest);
						_cvJobRequest.wait(lock);
					}
				}));
	}

	inline void JobSystemModule::Enqueue(const Job& job)
	{
		std::unique_lock<std::mutex> lk(_mutexJobPool);
		_jobPool.push(job);
		_inactiveNum++;
	}

	inline void JobSystemModule::Start()
	{
		std::unique_lock<std::mutex> lock(_mutexJobRequest);
		_cvJobRequest.notify_all();
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
