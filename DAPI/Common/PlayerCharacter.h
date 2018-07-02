#pragma once
#include"Structs.h"

namespace DAPI
{
	struct PlayerCharacter
	{
		PlayerCharacter(int pnum) : my_pnum{ pnum } { }

		char* walkPath() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return player[my_pnum]->walkpath; }
		int mode() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return player[my_pnum]->_pmode; }
		void walkToXY(int x, int y) {
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			ClrPlrPath(my_pnum);
			MakePlrPath(my_pnum, (unsigned char)x, (unsigned char)y, 1u);
			player[my_pnum]->destAction = -1;
		}
		void attack(int x, int y) {
			static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			MakePlrPath(my_pnum, (unsigned char)x, (unsigned char)y, 0);
			player[my_pnum]->destAction = 9;
			player[my_pnum]->destParam1 = x;
			player[my_pnum]->destParam2 = y;
		}
		void useItem(int i) {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto UseInvItem = reinterpret_cast<int(__fastcall *)(int pnum, int cii)>(0x41ED29);
			if (6 < i && i < 47)
			{
				if (player[my_pnum]->InvList[i - 7]._itype != -1)
					UseInvItem(0, i);
			}
			else if (46 < i && i < 57)
			{
				if (player[my_pnum]->SpdList[i - 47]._itype != -1)
					UseInvItem(0, i);
			}
		}
		void rangeAttack(int x, int y) {
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			ClrPlrPath(my_pnum);
			player[my_pnum]->destAction = 10;
			player[my_pnum]->destParam1 = x;
			player[my_pnum]->destParam2 = y;
		}
		void castSpell(int x, int y) {
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto CheckSpell = reinterpret_cast<bool(__fastcall *)(int id, int sn, char st, bool manaonly)>(0x457584);
			static auto UseScroll = reinterpret_cast<bool(__cdecl *)()>(0x41EB8B);
			static auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			if (player[my_pnum]->_pRSpell <= spell_id::SPL_BONESPIRIT &&
				spell_id::SPL_NULL < player[my_pnum]->_pRSpell)
			{
				if (player[my_pnum]->_pRSplType == spell_type::RSPLTYPE_SPELL)
				{
					if (CheckSpell(0, player[my_pnum]->_pRSpell, player[my_pnum]->_pRSplType, 0))
					{
						ClrPlrPath(0);
						player[my_pnum]->destAction = 12;
						player[my_pnum]->destParam1 = (unsigned char)x;
						player[my_pnum]->destParam2 = (unsigned char)y;
						player[my_pnum]->destParam3 = GetSpellLevel(0, player[my_pnum]->_pRSpell);
						player[my_pnum]->_pSplFrom = 0;
						player[my_pnum]->_pSpell = player[my_pnum]->_pRSpell;
						player[my_pnum]->_pSplType = player[my_pnum]->_pRSplType;
					}
				}
				else if (player[my_pnum]->_pRSplType == spell_type::RSPLTYPE_CHARGES)
				{
					ClrPlrPath(0);
					player[my_pnum]->destAction = 12;
					player[my_pnum]->destParam1 = x;
					player[my_pnum]->destParam2 = y;
					player[my_pnum]->destParam3 = GetSpellLevel(0, player[my_pnum]->_pRSpell);
					player[my_pnum]->_pSplFrom = 0;
					player[my_pnum]->_pSpell = player[my_pnum]->_pRSpell;
					player[my_pnum]->_pSplType = player[my_pnum]->_pRSplType;
				}
				else if (player[my_pnum]->_pRSplType == spell_type::RSPLTYPE_SCROLL)
				{
					if (UseScroll())
					{
						ClrPlrPath(0);
						player[my_pnum]->destAction = 12;
						player[my_pnum]->destParam1 = x;
						player[my_pnum]->destParam2 = y;
						player[my_pnum]->destParam3 = GetSpellLevel(0, player[my_pnum]->_pRSpell);
						player[my_pnum]->_pSplFrom = 0;
						player[my_pnum]->_pSpell = player[my_pnum]->_pRSpell;
						player[my_pnum]->_pSplType = player[my_pnum]->_pRSplType;
					}
				}
			}
		}
		int worldX() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return player[my_pnum]->WorldX; }
		int worldY() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return player[my_pnum]->WorldY; }
		
		
	private:
		int my_pnum;

	};
}