#pragma once
#include"Globals.h"
#include"../Common/Enums.h"
#include<thread>
#include<set>

namespace DAPI
{
	__declspec(dllexport) void walk(int x, int y);
	__declspec(dllexport) void attack(int x, int y);
	__declspec(dllexport) void useItem(int i);
	__declspec(dllexport) void rangeAttack(int x, int y);
	__declspec(dllexport) void castSpell(int x, int y);
	__declspec(dllexport) void interactObject(ObjectStruct* target_object);
	__declspec(dllexport) bool isClosed(ObjectStruct* target_object);
	__declspec(dllexport) void pickupItem(ItemStruct* target_item);

	__declspec(dllexport) PlayerStruct* getPlayer(int pnum);

}