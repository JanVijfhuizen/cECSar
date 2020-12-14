#pragma once
#include "IModule.h"
#include <ctime>
#include <cstdint>

namespace game
{
	class TimeModule final : public cecsar::IModule
	{
	public:
		constexpr float GetDeltaTime() const;
		constexpr float GetTime() const;
		constexpr int32_t GetFPS() const;

		inline void Update();

	private:
		clock_t previous = clock_t();
		float _deltaTime = 1;
		float _time = 0;
		int32_t _fps = 0;
	};

	constexpr float TimeModule::GetDeltaTime() const
	{
		return _deltaTime;
	}

	constexpr float TimeModule::GetTime() const
	{
		return _time;
	}

	constexpr int32_t TimeModule::GetFPS() const
	{
		return _fps;
	}

	inline void TimeModule::Update()
	{
		const clock_t c = clock();

		_deltaTime = float(c - previous) / 1000;
		_time += _deltaTime;
		_fps = 1.0f / _deltaTime;

		previous = c;
	}
}
