#pragma once
#include "EntityFactory.h"
#include "Components/Transform.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"
#include "Helpers/TransformHelper.h"

namespace game
{
	class UndeadFactory final : public cecsar::EntityFactory<Transform, Renderer>
	{
	protected:
		cecsar::Cecsar* _cecsar = nullptr;
		RenderModule* _renderModule = nullptr;

		utils::SparseSet<Transform>* _transforms = nullptr;
		utils::SparseSet<Renderer>* _renderers = nullptr;

		void Initialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(int32_t index, Transform&, Renderer&) override;
	};

	inline void UndeadFactory::Initialize(cecsar::Cecsar& cecsar)
	{
		_cecsar = &cecsar;
		_renderModule = &cecsar.GetModule<RenderModule>();

		_transforms = &_cecsar->GetSet<Transform>();
		_renderers = &_cecsar->GetSet<Renderer>();
	}

	inline void UndeadFactory::OnConstruction(const int32_t index,
		Transform& transform, Renderer& renderer)
	{
		SDL_Texture* texture = _renderModule->GetTexture("Art/Undead.png");
		renderer.texture = texture;
		renderer.count = 3;

		const auto feet = _cecsar->AddEntity(2);
		const auto hands = _cecsar->AddEntity(2);

		for (int32_t i = 0; i < 2; ++i)
		{
			const int32_t feetIndex = feet[i];

			auto& feetTransform = _cecsar->AddComponent<Transform>(feetIndex);
			feetTransform.x = (2 * i - 1) * _renderModule->DEFAULT_IMAGE_SIZE * 2;
			feetTransform.z = -.05f;

			auto& feetRenderer = _cecsar->AddComponent<Renderer>(feetIndex);
			feetRenderer.texture = texture;
			feetRenderer.count = 3;
			feetRenderer.index = 1;

			feetRenderer.color.c4 = _mm_set_ps1(200);

			TransformHelper::SetParent(*_transforms, feetIndex, index);
			if (i == 1)
				feetRenderer.flip = SDL_FLIP_HORIZONTAL;
		}

		for (int32_t i = 0; i < 2; ++i)
		{
			const int32_t handIndex = hands[i];

			auto& handTransform = _cecsar->AddComponent<Transform>(handIndex);
			handTransform.x = (2 * i - 1) * _renderModule->DEFAULT_IMAGE_SIZE * 4;
			handTransform.y = _renderModule->DEFAULT_IMAGE_SIZE * 2;
			handTransform.z = .05f;

			auto& handRenderer = _cecsar->AddComponent<Renderer>(handIndex);
			handRenderer.texture = texture;
			handRenderer.count = 3;
			handRenderer.index = 2;

			handRenderer.color.c4 = _mm_set_ps1(225);

			TransformHelper::SetParent(*_transforms, handIndex, index);
			if (i == 1)
				handRenderer.flip = SDL_FLIP_HORIZONTAL;
		}

		delete[] feet;
		delete[] hands;
	}
}
