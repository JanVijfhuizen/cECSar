#pragma once
#include "IModule.h"
#include <ctime>
#include <cstdint>
#include <algorithm>

namespace game
{
	class TimeModule final : public cecsar::IModule
	{
	public:
		float maxFixedStep = .2f;
		float fixedDeltaTime = .025f;

		[[nodiscard]] constexpr float GetDeltaTime() const;
		[[nodiscard]] constexpr float GetTime() const;
		[[nodiscard]] constexpr int32_t GetFPS() const;

		[[nodiscard]] constexpr int32_t GetPhysicsSteps() const;

		inline void Update();

	private:
		clock_t previous = clock_t();
		float _deltaTime = 1;

		float _time = 0;
		int32_t _fps = 0;

		float _accumulator = 0;
		int32_t _fixedSteps = 0;
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

	constexpr int32_t TimeModule::GetPhysicsSteps() const
	{
		return _fixedSteps;
	}

	inline void TimeModule::Update()
	{
		const clock_t c = clock();

		_deltaTime = float(c - previous) / 1000;
		_time += _deltaTime;
		_fps = 1.0f / _deltaTime;

		previous = c;

		// Calculate physics steps.
		_accumulator += std::min(_deltaTime, maxFixedStep);
		_fixedSteps = _accumulator / fixedDeltaTime;
		_accumulator -= fixedDeltaTime * _fixedSteps;
	}
}
