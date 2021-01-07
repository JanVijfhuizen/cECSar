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
	OnInitialize(cecsar);
}

void game::StandardFactory::OnConstruction(
	cecsar::Cecsar& cecsar, const cecsar::EntityInfo& info)
{
	const int32_t index = info.index;
	for (auto& imp : _imps)
		imp.second->Construct(cecsar, index);
}
