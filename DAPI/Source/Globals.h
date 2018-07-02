#pragma once
#include"../Common/Structs.h"

namespace DAPI
{
	typedef unsigned __int32 u32;

	static const u32 hook_function = 0x408974;//0x46886B;

	auto inline player_do_spell = reinterpret_cast<int(*)(int)>(0x44EAC6);
	auto inline doAttack = reinterpret_cast<int(*)(int)>(0x44E6A6); //May not need.
	auto inline StartWalk = reinterpret_cast<void(*)(int, int, int, int, int, int, int)>(0x44C1E2); //int pnum, int xvel, int yvel, int xadd, int yadd, int enddir, int sdir
	auto inline StartAttack = reinterpret_cast < void(*)(int, int)>(0x44C81E);
	auto inline PosOkPlayer = reinterpret_cast<bool(*)(int, int, int)>(0x44FDBA); //int pnum, int px, int py
	auto inline MakePlrPath = reinterpret_cast<int(*)(int, int, int, unsigned char)>(0x44FE9E); //int pnum, int xx, int yy, unsigned char endspace)
	auto inline FindPath = reinterpret_cast<int(*)(bool, int, int, int, int, int, char*)>(0x4493D4); //func, int PosOkArg, int sx, int sy, int dx, int dy, char *path
	auto inline dMonster = reinterpret_cast<int(*)[112][112]>(0x52D208);
	auto inline monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D24C);
	auto inline NetSendCmdString = reinterpret_cast<void(*)(int, const char*)>(0x43D064);
	auto inline ClrPlrPath = reinterpret_cast<void(*)(int pnum)>(0x44FD8A);
	auto inline UseInvItem = reinterpret_cast<int(*)(int pnum, int cii)>(0x41ED29);
	auto inline GetManaAmount = reinterpret_cast<int(*)(int id, int sn)>(0x45744E);
	auto inline CheckSpell = reinterpret_cast<bool(*)(int id, int sn, char st, bool manaonly)>(0x457584);
	auto inline GetSpellLevel = reinterpret_cast<int(*)(int id, int sn)>(0x428A99);
	auto inline UseScroll = reinterpret_cast<bool(__cdecl *)()>(0x41EB8B);
	static auto inline object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
	static auto inline item = reinterpret_cast<ItemStruct(*)[127]>(0x635A28);
	static auto inline dItem = reinterpret_cast<int(*)[112][112]>(0x5C9A10);

	static auto inline trigflag = reinterpret_cast<int(*)[5]>(0x6ABAC8);
	static auto inline trigs = reinterpret_cast<TriggerStruct(*)[5]>(0x6ABAE0);

}