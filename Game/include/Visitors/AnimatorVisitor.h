#pragma once
#include <Components/Renderer.h>
#include <Components/Animator.h>
#include "Utils/Mathf.h"

namespace game
{
	struct AnimatorVisitor final
	{
		struct Info final
		{
			Animator& animator;
			Renderer& renderer;
			float deltaTime = 0;
		};

		const Info& info;

		constexpr AnimatorVisitor(const Info& info);

		inline void operator()(const Animator::Simple& simple) const;
		inline void operator()(const Animator::Advanced& advanced) const;

	private:
		inline void Update() const;
	};

	constexpr AnimatorVisitor::AnimatorVisitor(const Info& info) :
		info(info)
	{
		
	}

	inline void AnimatorVisitor::operator()(const Animator::Simple& simple) const
	{
		auto& animator = info.animator;
		auto& renderer = info.renderer;

		const int32_t start = simple.start;
		const int32_t stop = simple.stop == -1 ? renderer.count - 1 : simple.stop;

		// Update linear interpolation.
		float& lerp = info.animator.lerp;
		lerp += info.deltaTime * animator.speed / (stop - start);

		// Get frame index.
		const int32_t index = utils::Mathf::Lerp(start, stop, lerp);
		renderer.index = index;

		Update();
	}

	inline void AnimatorVisitor::operator()(const Animator::Advanced& advanced) const
	{
		auto& animator = info.animator;
		auto& renderer = info.renderer;

		auto& keyFrames = advanced.keyFrames;
		const int32_t keyCount = keyFrames.size();

		// Update linear interpolation.
		float& lerp = info.animator.lerp;
		lerp += info.deltaTime * animator.speed / std::max(1, keyCount);

		// Get frame index.
		for (auto& frame : keyFrames)
		{
			if (frame.max > lerp)
				continue;

			renderer.index = frame.index;
			break;
		}

		Update();
	}

	inline void AnimatorVisitor::Update() const
	{
		auto& animator = info.animator;

		// Pause if not finished and not repeating.
		if (animator.lerp >= 1 && !animator.repeat) 
		{
			animator.paused = true;
			return;
		}

		animator.lerp = fmod(animator.lerp, 1.0f);
	}
}
