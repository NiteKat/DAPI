#include"ExampleAIModule.h"


void ExampleAIModule::onFrame()
{
	my_player = DAPI::player(0);
	if (my_player.mode() == DAPI::player_mode::PLAYER_MODE_STANDING)
	{
		my_player.walkToXY(my_player.worldX(), my_player.worldY() - 1);
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX(), my_player.worldY() + 1);
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY() - 1);
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY());
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() - 1, my_player.worldY() + 1);
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY() - 1);
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY());
		if (my_player.walkPath()[0] == -1)
			my_player.walkToXY(my_player.worldX() + 1, my_player.worldY() + 1);
	}
}