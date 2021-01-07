#pragma once
#include "Factories/IFactoryImp.h"
#include "Components/Renderer.h"
#include "Modules/RenderModule.h"

namespace game
{
	class StandardRendererImp : public FactoryImp<Renderer>
	{
	public:
		std::string path{};

		void PreInitialize(cecsar::Cecsar& cecsar) override;
		void OnConstruction(cecsar::Cecsar& cecsar,
			Renderer& component, int32_t index) override;

	protected:
		RenderModule& GetRenderModule() const;

	private:
		RenderModule* _renderModule = nullptr;
	};
}
