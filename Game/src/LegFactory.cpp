﻿#include "Factories/Humanoids/LegFactory.h"
#include "Components/Leg.h"

void game::LegFactory::OnInitializeCustom(cecsar::Cecsar& cecsar)
{
	LimbFactory::OnInitializeCustom(cecsar);

	offset = { 16, 8, -.01f };
	DefineImplementation<Leg>();
}
