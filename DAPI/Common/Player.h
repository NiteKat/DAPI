#pragma once
#include"Structs.h"
#include"Control.h"

namespace DAPI
{
	struct player
	{
		player(int pnum) : my_player{ getPlayer(pnum) } { }

		char* walkPath() { return my_player->walkpath; }
		int worldX() { return my_player->WorldX; }
		int worldY() { return my_player->WorldY; }
		int mode() { return my_player->_pmode; }
		void walkToXY(int x, int y) { walk(x, y); }

	private:
		PlayerStruct* my_player;
		__declspec(dllimport) PlayerStruct* getPlayer(int pnum);
		__declspec(dllimport) void walk(int x, int y);
	};
}