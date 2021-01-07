#include <Factories/StandardFactory.h>
#include "Factories/Implementations/StandardRendererImp.h"

game::StandardFactory::~StandardFactory()
{
	for (auto& imp : _imps)
		delete imp.second;
}

void game::StandardFactory::Initialize(cecsar::Cecsar& cecsar)
{
	_cecsar = &cecsar;
	OnInitializeCustom(cecsar);
	for (auto& imp : _imps)
		imp.second->PostInitialize(cecsar);
}

void game::StandardFactory::OnInitializeCustom(cecsar::Cecsar&)
{

}

void game::StandardFactory::OnConstruction(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	const int32_t index = info.index;
	for (auto& imp : _imps)
		imp.second->Construct(cecsar, index);
	OnConstructionCustom(cecsar, info);
}

void game::StandardFactory::OnConstructionCustom(
	cecsar::Cecsar&, const cecsar::EntityInfo&)
{

}
