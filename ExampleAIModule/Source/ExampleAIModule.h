#pragma once
#include"../../DAPI/Include/DAPI.h"

struct ExampleAIModule
{
	void onFrame();

	DAPI::player my_player;
};