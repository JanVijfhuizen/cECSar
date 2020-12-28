#include <Factories/Implementations/StandardRendererImp.h>

void game::StandardRendererImp::Initialize(cecsar::Cecsar& cecsar)
{
	_renderModule = &cecsar.GetModule<RenderModule>();
}

void game::StandardRendererImp::OnConstruction(
	cecsar::Cecsar&, Renderer& component, int32_t)
{
	component = prototype;
	component.texture = _renderModule->GetTexture(path);
}

game::RenderModule& game::StandardRendererImp::GetRenderModule() const
{
	return *_renderModule;
}