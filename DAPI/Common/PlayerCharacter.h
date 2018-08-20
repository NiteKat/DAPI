#pragma once
#include"Structs.h"
#include"Monster.h"
#include"Object.h"
#include"Door.h"
#include"Item.h"
#include<vector>
#include<sstream>
#include"Towner.h"
#include"Point.h"

namespace DAPI
{
	struct PlayerCharacter
	{
		PlayerCharacter() : my_pnum{ 0 } { }
		PlayerCharacter(int pnum) : my_pnum{ pnum } { }
		void attack(int x, int y) {
			static auto NetSendCmdLoc = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);
			NetSendCmdLoc(1u, static_cast<unsigned char>(_cmd_id::CMD_ATTACKXY), x, y);
			/*static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			switch (static_cast<item_type>((*player)[my_pnum].InvBody[4]._itype))
			{
			case item_type::ITYPE_BOW:
				ClrPlrPath(my_pnum);
				(*player)[my_pnum].destAction = 10;
				(*player)[my_pnum].destParam1 = (unsigned char)x;
				(*player)[my_pnum].destParam2 = (unsigned char)y;
				break;
			default:
				MakePlrPath(my_pnum, (unsigned char)x, (unsigned char)y, 0);
				(*player)[my_pnum].destAction = 9;
				(*player)[my_pnum].destParam1 = x;
				(*player)[my_pnum].destParam2 = y;
			}*/
		}
		void attack(Monster target) {
			static auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			int mid = target.id();
			if (mid != -1)
				NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ATTACKID), mid);
			/*static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			int Xdif = abs((*player)[my_pnum].WorldX - target.futurex());
			int Ydif = abs((*player)[my_pnum].WorldY - target.futurey());
			switch (static_cast<item_type>((*player)[my_pnum].InvBody[4]._itype))
			{
			case item_type::ITYPE_BOW:
				ClrPlrPath(my_pnum);
				(*player)[my_pnum].destAction = 22;
				(*player)[my_pnum].destParam1 = target.id() - 1;
				break;
			default:
				if (Xdif > 1 || Ydif > 1)
					MakePlrPath(my_pnum, target.futurex(), target.futurey(), 0);
				(*player)[my_pnum].destAction = 20;
				(*player)[my_pnum].destParam1 = (unsigned short)target.id() - 1;
			}*/
		}
		int baseDexterity() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseDex; }
		int baseMagic() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseMag; }
		int baseStrength() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseStr; }
		int baseVitality() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseVit; }
		//Broken, need to fix. Use castSpell(Monster target) instead.
		void castSpell(int x, int y) {
			static auto NetSendCmdLocParam2 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1, int wParam2)>(0x43C928);
			static auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if (*pcurs == 1)
				NetSendCmdLocParam2(1u, static_cast<unsigned char>(_cmd_id::CMD_SPELLXY), static_cast<unsigned char>(x), static_cast<unsigned char>(y), 21720 * my_pnum, GetSpellLevel(my_pnum, (*player)[my_pnum]._pRSpell));
			/*static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto CheckSpell = reinterpret_cast<bool(__fastcall *)(int id, int sn, char st, bool manaonly)>(0x457584);
			static auto UseScroll = reinterpret_cast<bool(__cdecl *)()>(0x41EB8B);
			static auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			if ((*player)[my_pnum]._pRSpell <= static_cast<int>(spell_id::SPL_BONESPIRIT) &&
				static_cast<int>(spell_id::SPL_NULL) < (*player)[my_pnum]._pRSpell)
			{
				if ((*player)[my_pnum]._pRSplType == static_cast<int>(spell_type::RSPLTYPE_SPELL))
				{
					if (CheckSpell(0, (*player)[my_pnum]._pRSpell, (*player)[my_pnum]._pRSplType, 0))
					{
						ClrPlrPath(0);
						(*player)[my_pnum].destAction = 12;
						(*player)[my_pnum].destParam1 = (unsigned char)x;
						(*player)[my_pnum].destParam2 = (unsigned char)y;
						(*player)[my_pnum].destParam3 = GetSpellLevel(0, (*player)[my_pnum]._pRSpell);
						(*player)[my_pnum]._pSplFrom = 0;
						(*player)[my_pnum]._pSpell = (*player)[my_pnum]._pRSpell;
						(*player)[my_pnum]._pSplType = (*player)[my_pnum]._pRSplType;
					}
				}
				else if ((*player)[my_pnum]._pRSplType == static_cast<int>(spell_type::RSPLTYPE_CHARGES))
				{
					ClrPlrPath(0);
					(*player)[my_pnum].destAction = 12;
					(*player)[my_pnum].destParam1 = x;
					(*player)[my_pnum].destParam2 = y;
					(*player)[my_pnum].destParam3 = GetSpellLevel(0, (*player)[my_pnum]._pRSpell);
					(*player)[my_pnum]._pSplFrom = 0;
					(*player)[my_pnum]._pSpell = (*player)[my_pnum]._pRSpell;
					(*player)[my_pnum]._pSplType = (*player)[my_pnum]._pRSplType;
				}
				else if ((*player)[my_pnum]._pRSplType == static_cast<int>(spell_type::RSPLTYPE_SCROLL))
				{
					if (UseScroll())
					{
						ClrPlrPath(0);
						(*player)[my_pnum].destAction = 12;
						(*player)[my_pnum].destParam1 = x;
						(*player)[my_pnum].destParam2 = y;
						(*player)[my_pnum].destParam3 = GetSpellLevel(0, (*player)[my_pnum]._pRSpell);
						(*player)[my_pnum]._pSplFrom = 0;
						(*player)[my_pnum]._pSpell = (*player)[my_pnum]._pRSpell;
						(*player)[my_pnum]._pSplType = (*player)[my_pnum]._pRSplType;
					}
				}
			}*/
		}
		void castSpell(Monster target)
		{
			static auto NetSendCmdParam3 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1, unsigned short wParam2, int wParam3)>(0x43CA04);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if (*pcurs == 1)
				NetSendCmdParam3(1u, static_cast<unsigned char>(_cmd_id::CMD_SPELLID), target.id(), (*player)[my_pnum]._pRSpell, GetSpellLevel(my_pnum, (*player)[my_pnum]._pRSpell));
		}

		int characterLevel() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pLevel; }
		int dLevel() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].plrlevel; }
		bool dropIteminCursor() {
			auto TryInvPut = reinterpret_cast<int(__cdecl *)()>(0x41E2F9);
			auto NetSendCmdPItem = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43CCF8);
			static auto SetCursor = reinterpret_cast<void(__fastcall *)(int i)>(0x40746B);
			auto pcurs = reinterpret_cast<int*>(0x4B8CD0);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (12 <= *pcurs)
			{
				if (!TryInvPut())
					return false;
				NetSendCmdPItem(1u, static_cast<unsigned char>(_cmd_id::CMD_PUTITEM), (*player)[my_pnum].WorldX, (*player)[my_pnum].WorldY);
				SetCursor(static_cast<int>(cursor_id::CURSOR_HAND));
				return true;
			}
			return false;
		}
		std::vector<Item> getBeltItems() {
			std::vector<Item> return_value;
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			for (int i = 0; i < 8; i++)
			{
				if ((*player)[my_pnum].SpdList[i]._itype != -1)
				{
					Item new_item(&((*player)[my_pnum].SpdList[i]));
					return_value.push_back(new_item);
				}
				else
				{
					Item no_item;
					return_value.push_back(no_item);
				}
			}
			return return_value;
		}
		_ui_classes getClass() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (_ui_classes)(*player)[my_pnum]._pClass;
		}
		Item getEquippedItem(equip_slot location)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (static_cast<int>(location) > -1 && static_cast<int>(location) < 7 && (*player)[my_pnum].InvBody[static_cast<int>(location)]._itype != static_cast<int>(item_type::ITYPE_NONE))
			{
				Item holditem(&(*player)[my_pnum].InvBody[static_cast<int>(location)]);
				return holditem;
			}
			else
			{
				Item emptyitem;
				return emptyitem;
			}
		}
		char* getInventoryGrid() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (*player)[my_pnum].InvGrid;
		}
		std::vector<Item> getInventoryItems() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			std::vector<Item> return_value;
			for (int i = 0; i < 40; i++)
			{
				if ((*player)[my_pnum].InvList[i]._itype != static_cast<int>(item_type::ITYPE_NONE))
				{
					Item item_to_add(&(*player)[my_pnum].InvList[i]);
					return_value.push_back(item_to_add);
				}
			}
			return return_value;
		}
		Item getItemInCursor()
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if (*pcurs > 12)
			{
				Item holditem(&(*player)[my_pnum].HoldItem);
				return holditem;
			}
			Item noItem;
			return noItem;
		}
		std::vector<spell_id> getLearnedSpells()
		{
			std::vector<spell_id> return_value;
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			for (int i = 0; i < 37; i++)
			{
				if ((*player)[my_pnum]._pSplLvl[i] > 0)
					return_value.push_back((spell_id)i);
			}
			return return_value;
		}

		spell_id getRightClickSpell()
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (spell_id)(*player)[my_pnum]._pRSpell;
		}

		int getRightClickSpellManaCost()
		{
			static auto GetManaAmount = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x45744E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor(GetManaAmount(my_pnum, (*player)[my_pnum]._pRSpell) / 64));
		}

		int getSpellLevel(spell_id id)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (static_cast<int>(id) < 37)
				return (*player)[my_pnum]._pSplLvl[static_cast<int>(id)];
			else
				return -1;
		}

		int gold() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (*player)[my_pnum]._pGold;
		}

		int hitPoints() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pHitPoints / 64));
		}
		bool increaseDexterity() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				(*player)[my_pnum]._pBaseDex++;
				(*player)[my_pnum]._pDexterity++;
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}
		bool increaseMagic() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				(*player)[my_pnum]._pBaseMag++;
				(*player)[my_pnum]._pMagic++;
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}
		bool increaseStrength() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				(*player)[my_pnum]._pBaseStr++;
				(*player)[my_pnum]._pStrength++;
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}
		bool increaseVitality() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				(*player)[my_pnum]._pBaseVit++;
				(*player)[my_pnum]._pVitality++;
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}
		bool isHoldingItem()
		{
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			return *pcurs == 12;
		}
		int mana() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMana / 64));
		}
		int maxHitPoints() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMaxHP / 64));
		}
		int maxMana() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMaxMana / 64));
		}
		int mode() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pmode; }
		void openDoor(Door target) {
			static auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
			NetSendCmdLocParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_OPOBJXY), target.x(), target.y(), target.id());

			/*static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (!target.isOpen())
			{
				MakePlrPath(my_pnum, target.x(), target.y(), 0);
				(*player)[my_pnum].destAction = 13;
				(*player)[my_pnum].destParam1 = target.id();
			}*/
		}
		void operateObject(Object target) {
			static auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
			static auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			//NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_OPERATEOBJ), target.id());
			NetSendCmdLocParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_OPOBJXY), target.x(), target.y(), target.id());
			/*static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (target.isSolid())
				MakePlrPath(my_pnum, target.x(), target.y(), 0);
			else
				MakePlrPath(my_pnum, target.x(), target.y(), 1u);
			(*player)[my_pnum].destAction = 13;
			(*player)[my_pnum].destParam1 = target.id();*/
		}
		void pickupItem(Item target) {
			static auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
			NetSendCmdLocParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_GOTOAGETITEM), target.x(), target.y(), target.groundId());
			/*static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			int ground_id = target.groundId();
			if (ground_id != -1)
			{
				MakePlrPath(my_pnum, target.x(), target.y(), 0);
				(*player)[my_pnum].destAction = 16;
				(*player)[my_pnum].destParam1 = ground_id;
			}*/
		}
		bool putCursorItem(int target)
		{
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto SetCursor = reinterpret_cast<void(__fastcall *)(int i)>(0x40746B);
			static auto CalcPlrInv = reinterpret_cast<void(__fastcall *)(int p, bool Loadgfx)>(0x41FD3E);
			static auto CheckItemStats = reinterpret_cast<void(__fastcall *)(int pnum)>(0x41D8BF);
			if (-1 < target && target < 55 && *pcurs == 12 + (*player)[my_pnum].HoldItem._iCurs)
			{
				ItemStruct temp;
				temp._itype = static_cast<int>(item_type::ITYPE_NONE);
				if (target < 7)
				{
					if ((*player)[my_pnum].InvBody[target]._itype != static_cast<int>(item_type::ITYPE_NONE))
						temp = (*player)[my_pnum].InvBody[target];
					(*player)[my_pnum].InvBody[target] = (*player)[my_pnum].HoldItem;
				}
				else if (target < 47)
				{
					int grid_index;
					switch (static_cast<item_type>((*player)[my_pnum].HoldItem._itype))
					{
					case item_type::ITYPE_AMULET:
					case item_type::ITYPE_GOLD:
					case item_type::ITYPE_MISC:
					case item_type::ITYPE_RING:
						if ((*player)[my_pnum].InvGrid[target - 7] == '\0')
						{
							int index = putInInvList((*player)[my_pnum].HoldItem);
							if (index != 0)
								(*player)[my_pnum].InvGrid[target - 7] = index;
							else
								return false;
						}
						else
						{
							int index = (*player)[my_pnum].InvGrid[target - 7];
							index = abs(index) - 1;
							temp = (*player)[my_pnum].InvList[index];
							(*player)[my_pnum].InvList[index] = (*player)[my_pnum].HoldItem;
							(*player)[my_pnum].HoldItem = temp;
							clearInvGridForItem(index + 1);
							(*player)[my_pnum].InvGrid[target - 7] = index + 1;
						}
						break;
					case item_type::ITYPE_MACE:
					case item_type::ITYPE_SWORD:
						grid_index = target - 7;
						if (grid_index >= 19)
							return false;
						else
						{
							if ((*player)[my_pnum].InvGrid[target - 7] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 3] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 13] == '\0')
							{
								int index = putInInvList((*player)[my_pnum].HoldItem);
								if (index != 0)
								{
									(*player)[my_pnum].InvGrid[target - 7] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 3] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 13] = index;
								}
								else
									return false;
							}
							else
							{
								int overlapping_index = 0;
								for (int i = 0; i < 40; i++)
								{
									if (i == target - 7 || i == target + 3 || i == target + 13)
									{
										if ((*player)[my_pnum].InvGrid[i] != '\0')
										{
											if (overlapping_index && overlapping_index != abs((*player)[my_pnum].InvGrid[i]))
											{
												return false;
											}
											else if (overlapping_index == 0)
											{
												overlapping_index = abs((*player)[my_pnum].InvGrid[i]);
											}
										}
									}
								}
								clearInvGridForItem(overlapping_index);
								(*player)[my_pnum].InvGrid[target - 7] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 3] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 13] = overlapping_index;
								temp = (*player)[my_pnum].InvList[overlapping_index - 1];
								(*player)[my_pnum].InvList[overlapping_index - 1] = (*player)[my_pnum].HoldItem;
							}
						}
						break;
					case item_type::ITYPE_AXE:
					case item_type::ITYPE_BOW:
					case item_type::ITYPE_HARMOR:
					case item_type::ITYPE_LARMOR:
					case item_type::ITYPE_MARMOR:
					case item_type::ITYPE_STAFF:
						grid_index = target - 7;
						if (grid_index == 9 || grid_index >= 19)
							return false;
						else
						{
							if ((*player)[my_pnum].InvGrid[target - 7] == '\0' &&
								(*player)[my_pnum].InvGrid[target - 6] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 3] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 4] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 13] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 14] == '\0')
							{
								int index = putInInvList((*player)[my_pnum].HoldItem);
								if (index != 0)
								{
									(*player)[my_pnum].InvGrid[target - 7] = -1 * index;
									(*player)[my_pnum].InvGrid[target - 6] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 3] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 4] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 13] = index;
									(*player)[my_pnum].InvGrid[target + 14] = -1 * index;
								}
								else
									return false;
							}
							else
							{
								int overlapping_index = 0;
								for (int i = 0; i < 40; i++)
								{
									if (i == target - 7 || i == target - 6 || i == target + 3 || i == target + 4 || i == target + 13 || i == target + 14)
									{
										if ((*player)[my_pnum].InvGrid[i] != '\0')
										{
											if (overlapping_index && overlapping_index != abs((*player)[my_pnum].InvGrid[i]))
											{
												return false;
											}
											else if (overlapping_index == 0)
											{
												overlapping_index = abs((*player)[my_pnum].InvGrid[i]);
											}
										}
									}
								}
								clearInvGridForItem(overlapping_index);
								(*player)[my_pnum].InvGrid[target - 7] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target - 6] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 3] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 4] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 13] = overlapping_index;
								(*player)[my_pnum].InvGrid[target + 14] = -1 * overlapping_index;
								temp = (*player)[my_pnum].InvList[overlapping_index - 1];
								(*player)[my_pnum].InvList[overlapping_index - 1] = (*player)[my_pnum].HoldItem;
							}
						}
					case item_type::ITYPE_HELM:
						grid_index = target - 7;
						if (grid_index == 9 || grid_index == 19 || grid_index >= 29)
							return false;
						else
						{
							if ((*player)[my_pnum].InvGrid[target - 7] == '\0' &&
								(*player)[my_pnum].InvGrid[target - 6] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 3] == '\0' &&
								(*player)[my_pnum].InvGrid[target + 4] == '\0')
							{
								int index = putInInvList((*player)[my_pnum].HoldItem);
								if (index != 0)
								{
									(*player)[my_pnum].InvGrid[target - 7] = -1 * index;
									(*player)[my_pnum].InvGrid[target - 6] = -1 * index;
									(*player)[my_pnum].InvGrid[target + 3] = index;
									(*player)[my_pnum].InvGrid[target + 4] = -1 * index;
								}
								else
									return false;
							}
							else
							{
								int overlapping_index = 0;
								for (int i = 0; i < 40; i++)
								{
									if (i == target - 7 || i == target - 6 || i == target + 3 || i == target + 4)
									{
										if ((*player)[my_pnum].InvGrid[i] != '\0')
										{
											if (overlapping_index && overlapping_index != abs((*player)[my_pnum].InvGrid[i]))
											{
												return false;
											}
											else if (overlapping_index == 0)
											{
												overlapping_index = abs((*player)[my_pnum].InvGrid[i]);
											}
										}
									}
								}
								clearInvGridForItem(overlapping_index);
								(*player)[my_pnum].InvGrid[target - 7] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target - 6] = -1 * overlapping_index;
								(*player)[my_pnum].InvGrid[target + 3] = overlapping_index;
								(*player)[my_pnum].InvGrid[target + 4] = -1 * overlapping_index;
								temp = (*player)[my_pnum].InvList[overlapping_index - 1];
								(*player)[my_pnum].InvList[overlapping_index - 1] = (*player)[my_pnum].HoldItem;
							}
						}

					}
				}
				else if (target < 55)
				{
					//belt
					if ((*player)[my_pnum].HoldItem._itype != static_cast<int>(item_type::ITYPE_MISC))
						return false;
					if ((*player)[my_pnum].SpdList[target - 47]._itype != static_cast<int>(item_type::ITYPE_NONE))
						temp = (*player)[my_pnum].SpdList[target - 47];
					(*player)[my_pnum].SpdList[target - 47] = (*player)[my_pnum].HoldItem;
				}
				if (temp._itype != static_cast<int>(item_type::ITYPE_NONE))
				{
					(*player)[my_pnum].HoldItem = temp;
					SetCursor((*player)[my_pnum].HoldItem._iCurs + 12);
				}
				else
				{
					SetCursor(1);
					if (target > 6 && target < 47)
						(*player)[my_pnum]._pNumInv += 1;
				}
				CalcPlrInv(my_pnum, 1u);
				CheckItemStats(my_pnum);
				return true;
			}
			else
				return false;
		}
		bool putInCursor(Item target)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto SetCursor = reinterpret_cast<void(__fastcall *)(int i)>(0x40746B);
			static auto CalcPlrInv = reinterpret_cast<void(__fastcall *)(int p, bool Loadgfx)>(0x41FD3E);
			static auto CheckItemStats = reinterpret_cast<void(__fastcall *)(int pnum)>(0x41D8BF);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if ((*player)[my_pnum]._pmode > static_cast<int>(player_mode::PLAYER_MODE_WALKING_3))
				return false;
			if (target.type() != static_cast<int>(item_type::ITYPE_NONE))
			{
				if (*pcurs == 1)
				{
					for (int i = 0; i < 55; i++)
					{
						if (i < 7)
						{
							Item temp(&(*player)[my_pnum].InvBody[i]);
							if (target == temp)
							{
								(*player)[my_pnum].HoldItem = (*player)[my_pnum].InvBody[i];
								(*player)[my_pnum].InvBody[i]._itype = static_cast<int>(item_type::ITYPE_NONE);
								CalcPlrInv(my_pnum, 1u);
								CheckItemStats(my_pnum);
								SetCursor((*player)[my_pnum].HoldItem._iCurs + 12);
								return true;
							}
						}
						else if (i < 47)
						{
							Item temp(&(*player)[my_pnum].InvList[i - 7]);
							if (target == temp)
							{
								(*player)[my_pnum].HoldItem = (*player)[my_pnum].InvList[i - 7];
								(*player)[my_pnum].InvList[i - 7]._itype = static_cast<int>(item_type::ITYPE_NONE);
								for (int j = 0; j < 40; j++)
								{
									if ((*player)[my_pnum].InvGrid[j] == target.inventoryIndex(my_pnum) - 6 ||
										(*player)[my_pnum].InvGrid[j] == -1 * (target.inventoryIndex(my_pnum) - 6))
										(*player)[my_pnum].InvGrid[j] = '\0';
								}
								CalcPlrInv(my_pnum, 1u);
								CheckItemStats(my_pnum);
								SetCursor((*player)[my_pnum].HoldItem._iCurs + 12);
								(*player)[my_pnum]._pNumInv -= 1;
								return true;
							}
						}
					}
				}
			}
			return false;
		}
		bool repair(Item target) {
			static auto DoRepair = reinterpret_cast<void(__fastcall *)(int pnum, int cii)>(0x422C9C);
			if (target.isValid() && target.inventoryIndex(my_pnum) > -1)
			{
				DoRepair(my_pnum, target.inventoryIndex(my_pnum));
				setCursor(cursor_id::CURSOR_HAND);
				return true;
			}
			setCursor(cursor_id::CURSOR_HAND);
			return false;
		}
		void selectStoreOption(STextStruct& option) {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			auto stextsel = reinterpret_cast<int*>(0x6A8A28);
			auto STextEnter = reinterpret_cast<void(*)()>(0x45BF34);
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			if (*stextflag){
				for (int i = 0; i < 24; i++) {
					std::string soption = option._sstr;
					std::string sstext = (*stext)[i]._sstr;
					if (soption == sstext) {
						*stextsel = i;
						STextEnter();
						break;
					}
				}
			}
		}
		bool setRightClickSpell(spell_id id)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (id == spell_id::SPL_REPAIR || id == spell_id::SPL_DISARM || id == spell_id::SPL_RECHARGE)
			{
				(*player)[my_pnum]._pRSpell = static_cast<int>(id);
				(*player)[my_pnum]._pRSplType = static_cast<int>(spell_type::RSPLTYPE_SKILL);
				(*player)[my_pnum]._pSpell = static_cast<int>(id);
				(*player)[my_pnum]._pSplType = static_cast<int>(spell_type::RSPLTYPE_SKILL);
				return true;
			}
			if (getSpellLevel(id) > 0)
			{
				(*player)[my_pnum]._pRSpell = static_cast<int>(id);
				(*player)[my_pnum]._pRSplType = static_cast<int>(spell_type::RSPLTYPE_SPELL);
				return true;
			}
			return false;
		}
		void shiftAttack(Point target) {
			auto NetSendCmdLoc = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);
			NetSendCmdLoc(1u, static_cast<unsigned char>(_cmd_id::CMD_SATTACKXY), target.x, target.y);
		}
		int statPoints() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pStatPts; }
		void talkToTowner(Towner towner) {
			static auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
			if (okToAct()) {
				if (-1 < towner.id() && towner.id() < 16)
					NetSendCmdLocParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_TALKXY), towner.x(), towner.y(), towner.id());
			}
		}
		char* walkPath() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].walkpath; }
		void walkToXY(int x, int y) {
			static auto NetSendCmdLoc = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);
			NetSendCmdLoc(1u, static_cast<unsigned char>(_cmd_id::CMD_WALKXY), x, y);
			/*static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto MakePlrPath = reinterpret_cast<int(__fastcall *)(int, int, int, unsigned char)>(0x44FE9E);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			ClrPlrPath(my_pnum);
			MakePlrPath(my_pnum, (unsigned char)x, (unsigned char)y, 1u);
			(*player)[my_pnum].destAction = -1;*/
		}
		void useItem(Item target)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto UseInvItem = reinterpret_cast<int(__fastcall *)(int pnum, int cii)>(0x41ED29);
			int inv_index = target.inventoryIndex(my_pnum);
			if (inv_index != -1)
				UseInvItem(my_pnum, inv_index);
		}
		void useItem(int i) {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto UseInvItem = reinterpret_cast<int(__fastcall *)(int pnum, int cii)>(0x41ED29);
			if (6 < i && i < 47)
			{
				if ((*player)[my_pnum].InvList[i - 7]._itype != -1)
					UseInvItem(my_pnum, i);
			}
			else if (46 < i && i < 57)
			{
				if ((*player)[my_pnum].SpdList[i - 47]._itype != -1)
					UseInvItem(my_pnum, i);
			}
		}
		void rangeAttack(int x, int y) {
			static auto ClrPlrPath = reinterpret_cast<void(__fastcall *)(int pnum)>(0x44FD8A);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			ClrPlrPath(my_pnum);
			(*player)[my_pnum].destAction = 10;
			(*player)[my_pnum].destParam1 = x;
			(*player)[my_pnum].destParam2 = y;
		}
		int worldX() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].WorldX; }
		int worldY() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].WorldY; }
		
		
	private:
		int my_pnum;
		int putInInvList(ItemStruct item) {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			for (int i = 0; i < 40; i++)
			{
				if ((*player)[my_pnum].InvList[i]._itype == static_cast<int>(item_type::ITYPE_NONE))
				{
					(*player)[my_pnum].InvList[i] = item;
					return i + 1;
				}
			}
			return 0;
		}
		void clearInvGridForItem(int index)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			for (int i = 0; i < 40; i++)
			{
				if (abs((*player)[my_pnum].InvGrid[i]) == index)
					(*player)[my_pnum].InvGrid[i] = '\0';
			}
		}
		void setCursor(cursor_id new_cursor) {
			static auto pcurs = reinterpret_cast<int(*)>(0x4E8CD0);
			*pcurs = static_cast<int>(new_cursor);
		}
		bool okToAct() {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			return !*stextflag;
		}
	};
}