#pragma once

#include "Cecsar.h"
#include "Module.h"
#include <iostream>

namespace jecs::example
{
	struct OGLRenderer
	{
		// Some OpenGL variables.
	};

	struct VulkanRenderer
	{
		// Some Vulkan variables.
	};

	// Some module.
	class RenderSystem : public jecs::Module<RenderSystem>
	{
	public:
		virtual void Update() {};
	};

	class OGLRenderSystem : public RenderSystem
	{
	public:
		void Update() override
		{
			// Do some OpenGL specific rendering.
			std::cout << "OpenGL" << std::endl;
		}
	};

	class VulkanRenderSystem : public RenderSystem
	{
	public:
		void Update() override
		{
			// Do some Vulkan specific rendering.
			std::cout << "Vulkan" << std::endl;
		}
	};

	int RunExampleAbstraction()
	{
		jecs::Cecsar cecsar;

		// Define implementations based on your platform / settings.
#ifdef DEBUG
		new OGLRenderSystem;
#else
		new VulkanRenderSystem;
#endif

		RenderSystem& sys = RenderSystem::Get();
		sys.Update();

		return 0;
	}
}