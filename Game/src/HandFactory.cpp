#include <Factories/Humanoids/HandFactory.h>
#include "Components/Joint.h"

void game::HandFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	LimbFactory::OnInitializeCustom(cecsar);

	offset = { 24, 32, -.01f };

	auto& joint = DefineImplementation<Joint>();
	joint.prototype.maxDistance = 16;
}
