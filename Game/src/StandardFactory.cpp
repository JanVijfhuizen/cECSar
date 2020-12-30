#include <Factories/StandardFactory.h>
#include "Factories/Implementations/StandardRendererImp.h"

game::StandardFactory::~StandardFactory()
{
	delete _renderImp;
	delete _transformImp;
	delete _colliderImp;
}

void game::StandardFactory::Initialize(cecsar::Cecsar& cecsar)
{
	_renderImp = SetRenderImp(cecsar);
	_transformImp = SetTransformImp(cecsar);
	_colliderImp = SetColliderImp(cecsar);

	_renderImp->Initialize(cecsar);
	_transformImp->Initialize(cecsar);
	_colliderImp->Initialize(cecsar);
}

void game::StandardFactory::OnConstruction(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	auto& renderer = cecsar.AddComponent<Renderer>(info.index);
	auto& transform = cecsar.AddComponent<Transform>(info.index);
	auto& collider = cecsar.AddComponent<Collider>(info.index);

	_renderImp->OnConstruction(cecsar, renderer, info.index);
	_transformImp->OnConstruction(cecsar, transform, info.index);
	_colliderImp->OnConstruction(cecsar, collider, info.index);
}

game::IFactoryImp<game::Renderer>* game::StandardFactory::SetRenderImp(cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<Renderer>;
}

game::IFactoryImp<game::Transform>* game::StandardFactory::SetTransformImp(cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<Transform>;
}

game::IFactoryImp<game::Collider>* game::StandardFactory::SetColliderImp(cecsar::Cecsar& cecsar)
{
	return new IFactoryImp<Collider>;
}
