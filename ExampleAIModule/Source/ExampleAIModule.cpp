#include"ExampleAIModule.h"

DAPI::Game test;
DAPI::PlayerCharacter my_player = test.self();

extern "C" __declspec(dllexport) void onFrame()
{
	auto trigs = test.test();
	if (!trigs)
	{
		if (my_player.mode() == DAPI::player_mode::PLAYER_MODE_STANDING)
			my_player.walkToXY(my_player.worldX(), my_player.worldY() + 1);
	}
	else if (my_player.mode() == DAPI::player_mode::PLAYER_MODE_STANDING)
	{
		my_player.walkToXY(my_player.worldX(), my_player.worldY() - 1);
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX(), my_player.worldY() + 1);
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY() - 1);
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY());
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY() + 1);
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY() - 1);
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY());
		if (my_player.walkPath()[0] == (char)-1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY() + 1);
	}
}