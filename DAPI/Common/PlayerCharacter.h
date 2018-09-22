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

		//Issues an attack order as if you left clicked on the monster
		//passed to the function.
		void attack(Monster target) {
			auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			int mid = target.id();
			if (mid != -1) {
				if (static_cast<item_type>((*player)[*myplr].InvBody[4]._itype) == item_type::ITYPE_BOW || static_cast<item_type>((*player)[*myplr].InvBody[5]._itype) == item_type::ITYPE_BOW)
					NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_RATTACKID), mid);
				else
					NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ATTACKID), mid);
			}
		}

		//Returns the base Dexterity of the character.
		int baseDexterity() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseDex; }

		//Returns the base Magic of the character.
		int baseMagic() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseMag; }

		//Returns the basae Strength of the character.
		int baseStrength() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseStr; }

		//Returns the baase Vitality of the character.
		int baseVitality() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pBaseVit; }

		//Casts the spell set as the right click spell, as if you right clicked (or shift right clicked)
		//with no monsters selected.
		void castSpell(int x, int y) {
			auto NetSendCmdLocParam2 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1, int wParam2)>(0x43C928);
			auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			auto UseScroll = reinterpret_cast<bool(__cdecl *)()>(0x41EB8B);
			if (*pcurs == 1) {
				if ((*player)[my_pnum]._pRSplType == static_cast<int>(spell_type::RSPLTYPE_SCROLL)) {
					if(UseScroll())
						NetSendCmdLocParam2(1u, static_cast<unsigned char>(_cmd_id::CMD_SPELLXY), static_cast<unsigned char>(x), static_cast<unsigned char>(y), (*player)[my_pnum]._pRSpell, GetSpellLevel(my_pnum, (*player)[my_pnum]._pRSpell));
				}
				else
					NetSendCmdLocParam2(1u, static_cast<unsigned char>(_cmd_id::CMD_SPELLXY), static_cast<unsigned char>(x), static_cast<unsigned char>(y), (*player)[my_pnum]._pRSpell, GetSpellLevel(my_pnum, (*player)[my_pnum]._pRSpell));
			}
		}

		//Casts the spell set as the right click spell, as if you right clicked with a monster selected.
		void castSpell(Monster target)
		{
			static auto NetSendCmdParam3 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1, unsigned short wParam2, int wParam3)>(0x43CA04);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto GetSpellLevel = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x428A99);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if (*pcurs == 1)
				NetSendCmdParam3(1u, static_cast<unsigned char>(_cmd_id::CMD_SPELLID), target.id(), (*player)[my_pnum]._pRSpell, GetSpellLevel(my_pnum, (*player)[my_pnum]._pRSpell));
		}

		//Returns the level of the character.
		int characterLevel() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pLevel; }

		//Broken, will crash game.
		void chat(std::string text) {
			auto NetSendCmdString = reinterpret_cast<void(*)(int, const char*)>(0x43D064);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			NetSendCmdString(1 << *myplr, text.data());
		}

		//Returns the dungeon level on which the character is located. 0 is returned for town.
		int dLevel() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].plrlevel; }

		//If an item is held in the cursor, it will be dropped on the ground if there is room.
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

		//Returns all of the items in the belt.
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

		//Returns the class of the character.
		_ui_classes getClass() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (_ui_classes)(*player)[my_pnum]._pClass;
		}

		//Returns the item equipped at the location passed to the function.
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

		//Returns the inventory grid, which exists as a char* [40]. 0-9 is the top row,
		//10-19 is the second row, 20-29 is the third row, and 30-34 is the bottom row.
		//The values indicate the index (plus one if positive, minus one if negative
		//in the inventory list of the item occupying that slot. If an item occupies more
		//than one square, one of the squares will be a positive value while the others
		//will be a negative value.
		char* getInventoryGrid() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (*player)[my_pnum].InvGrid;
		}

		//Returns all of the items sitting in the 10x4 inventory of the character.
		std::vector<Item> getInventoryItems() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			std::vector<Item> return_value;
			/*
			for (int i = 0; i < 40; i++)
			{
				if ((*player)[my_pnum].InvList[i]._itype != static_cast<int>(item_type::ITYPE_NONE))
				{
					for (int j = 0; j < 40; j++) {
						if (abs((*player)[my_pnum].InvGrid[j]) == )
					}
					Item item_to_add(&(*player)[my_pnum].InvList[i]);
					return_value.push_back(item_to_add);
				}
			}*/
			bool used[40] = { false };
			for (int i = 0; i < 40; i++) {
				int index = (*player)[my_pnum].InvGrid[i];
				if (index != 0 && !used[abs(index) - 1]) {
					return_value.push_back(Item(&(*player)[my_pnum].InvList[abs(index) - 1]));
					used[abs(index) - 1] = true;
				}
			}
			return return_value;
		}

		//Returns the item held in the cursor.
		Item getItemInCursor()
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			if (*pcurs >= 12)
			{
				Item holditem(&(*player)[my_pnum].HoldItem);
				return holditem;
			}
			Item noItem;
			return noItem;
		}

		//Returns all spells whose levels are greater than 0 for the character. These
		//are the spells that your character has learned, and can cast using Mana.
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

		//Returns the amount of Mana required to cast the spell passed to it.
		int getSpellManaCost(DAPI::spell_id spell) {
			auto GetManaAmount = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x45744E);
			return static_cast<int>(floor(GetManaAmount(my_pnum, static_cast<int>(spell)) / 64));

		}

		//Returns which spell is set as the right click spell.
		spell_id getRightClickSpell()
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (spell_id)(*player)[my_pnum]._pRSpell;
		}

		//Returns all spells that you can set to right click that you have as scrolls.
		std::vector<spell_id> getScrollSpells() {
			std::vector<spell_id> return_value;
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x1) {
				return_value.push_back(spell_id::SPL_FIREBOLT);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x2) {
				return_value.push_back(spell_id::SPL_HEAL);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x4) {
				return_value.push_back(spell_id::SPL_LIGHTNING);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x8) {
				return_value.push_back(spell_id::SPL_FLASH);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x10) {
				return_value.push_back(spell_id::SPL_IDENTIFY);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x20) {
				return_value.push_back(spell_id::SPL_FIREWALL);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x40) {
				return_value.push_back(spell_id::SPL_TOWN);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x80) {
				return_value.push_back(spell_id::SPL_STONE);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x100) {
				return_value.push_back(spell_id::SPL_INFRA);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x200) {
				return_value.push_back(spell_id::SPL_RNDTELEPORT);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x400) {
				return_value.push_back(spell_id::SPL_MANASHIELD);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x800) {
				return_value.push_back(spell_id::SPL_FIREBALL);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x1000) {
				return_value.push_back(spell_id::SPL_GUARDIAN);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x2000) {
				return_value.push_back(spell_id::SPL_CHAIN);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x4000) {
				return_value.push_back(spell_id::SPL_WAVE);
			}
			if (((*player)[*myplr]._pScrlSpells[0]) & 0x8000) {
				return_value.push_back(spell_id::SPL_DOOMSERP);
			}
			if (((*player)[*myplr]._pScrlSpells[1]) & 0x1) {
				return_value.push_back(spell_id::SPL_TELEKINESIS);
			}
			if (((*player)[*myplr]._pScrlSpells[1]) & 0x2) {
				return_value.push_back(spell_id::SPL_HEALOTHER);
			}
			if (((*player)[*myplr]._pScrlSpells[1]) & 0x4) {
				return_value.push_back(spell_id::SPL_FLARE);
			}
			if (((*player)[*myplr]._pScrlSpells[1]) & 0x8) {
				return_value.push_back(spell_id::SPL_BONESPIRIT);
			}
			return return_value;
		}

		//Gets the Mana required to cast the right click spell.
		int getRightClickSpellManaCost()
		{
			auto GetManaAmount = reinterpret_cast<int(__fastcall *)(int id, int sn)>(0x45744E);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor(GetManaAmount(my_pnum, (*player)[my_pnum]._pRSpell) / 64));
		}

		//Returns the level of the spell passed to the function.
		int getSpellLevel(spell_id id)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if (static_cast<int>(id) < 37)
				return (*player)[my_pnum]._pSplLvl[static_cast<int>(id)];
			else
				return -1;
		}

		//Returns the total amount of gold the character has.
		int gold() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return (*player)[my_pnum]._pGold;
		}

		//Returns the current Life of the character.
		int hitPoints() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pHitPoints / 64));
		}

		//Returns the index at which the character is stored in the Player array.
		int id() {
			return my_pnum;
		}

		//If the character has stat points to spend, spends one point on Dexterity.
		bool increaseDexterity() {
			auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ADDDEX), 1u);
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}

		//If the character has stat points to spend, spends one point on Magic.
		bool increaseMagic() {
			auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ADDMAG), 1u);
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}

		//If the character has stat points to spend, spends one point on Strength.
		bool increaseStrength() {
			auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ADDSTR), 1u);
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}

		//If the character has stat points to spend, spends one point on Vitality.
		bool increaseVitality() {
			auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			if ((*player)[my_pnum]._pStatPts > 0)
			{
				NetSendCmdParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_ADDVIT), 1u);
				(*player)[my_pnum]._pStatPts--;
				return true;
			}
			return false;
		}

		//Returns true if you have an item held in the cursor.
		bool isHoldingItem()
		{
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			return *pcurs == 12;
		}

		//Returns the current Mana value for the character.
		int mana() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMana / 64));
		}

		//Returns the maximum Life for the character.
		int maxHitPoints() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMaxHP / 64));
		}

		//Returns the maximum Mana for the character.
		int maxMana() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			return static_cast<int>(floor((*player)[my_pnum]._pMaxMana / 64));
		}

		//Returns the current mode for the character.
		int mode() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pmode; }

		//Issues a command to open the door passed to the function, as if you left clicked on the door.
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

		//Issues a command to operate the object passed to the function, as if you left clicked the object.
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

		//Issues a command to auto get the item passed to the function, as if you left clicked the item
		//with your inventory closed.
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

		//Picks up the item in the equip_slot passed to the function, putting it into the cursor.
		//Recommneded to use the version that takes an item instead of an equip_slot parameter.
		void putCursorItem(int target)
		{
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			auto CheckInvPaste = reinterpret_cast<void(__fastcall *)(int pnum, int mx, int my)>(0x41C783);
			auto InvRect = reinterpret_cast<InvXY(*)[73]>(0x47AE60);
			if (12 <= *pcurs && my_pnum == *myplr && static_cast<equip_slot>(target) <= equip_slot::BELT8) {
				int mx = 0;
				int my = 0;
				if (static_cast<equip_slot>(target) == equip_slot::HEAD) {
					mx = (*InvRect)[0].X + 1;//456;//
					my = (*InvRect)[0].Y - 1;//5;//
				}
				else if (static_cast<equip_slot>(target) == equip_slot::LEFTRING) {
					mx = (*InvRect)[5].X + 2;
					my = (*InvRect)[5].Y - 15;
				}
				else if (static_cast<equip_slot>(target) == equip_slot::RIGHTRING) {
					mx = (*InvRect)[4].X + 2;
					my = (*InvRect)[4].Y - 15;
				}
				else if (static_cast<equip_slot>(target) == equip_slot::AMULET) {
					mx = (*InvRect)[6].X + 2;
					my = (*InvRect)[6].Y - 15;
				}
				else if (static_cast<equip_slot>(target) == equip_slot::LEFTHAND) {
					mx = (*InvRect)[7].X + 1; //571;//
					my = (*InvRect)[7].Y - 1; //78;// 
				}
				else if (static_cast<equip_slot>(target) == equip_slot::RIGHTHAND) {
					mx = (*InvRect)[13].X + 1;//339;// 
					my = (*InvRect)[13].Y - 1;//79;//
				}
				else if (static_cast<equip_slot>(target) == equip_slot::BODY) {
					mx = (*InvRect)[19].X + 1;//458; //
					my = (*InvRect)[19].Y - 1;//80;// 
				}
				else if (target < 47) {
					int index = 18 + target;
					mx = (*InvRect)[index].X + 2; //338 + ((target - 7) % 10) * 30; // //338
					my = (*InvRect)[index].Y - 20;//224 + ((target - 7) / 10) * 30;; //224
				}
				else if (target <= 54) {
						int index = 18 + target;
						mx = 210 + (target - 47) * 30;//(*InvRect)[index].X + 10;
						my = 370;//(*InvRect)[index].Y - 10;
				}
				CheckInvPaste(*myplr, mx, my);
			}

			/*static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
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
				return false;*/
		}

		//Picks up the item passed to the function, and puts it in the cursor, so long as the item is
		//either equipped, in the character's inventory, or in their belt.
		void putInCursor(Item target)
		{
			auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto CheckInvCut = reinterpret_cast<void(__fastcall *)(int pnum, int mx, int my)>(0x41D378);
			auto InvRect = reinterpret_cast<InvXY(*)[73]>(0x47AE60);
			if (static_cast<item_type>(target.type()) == item_type::ITYPE_NONE)
				return;
			int mx = 0;
			int my = 0;
			if (*pcurs < 12 && my_pnum == *myplr) {
				for (int i = 0; i < 55; i++) {
					if (i < 7) {
						Item temp(&(*player)[my_pnum].InvBody[i]);
						if (target == temp)
						{
							switch (static_cast<equip_slot>(i)) {
							case equip_slot::HEAD:
								mx = (*InvRect)[0].X + 1;//456;//
								my = (*InvRect)[0].Y - 1;//5;//
								break;
							case equip_slot::LEFTRING:
								mx = (*InvRect)[4].X + 1;
								my = (*InvRect)[4].Y - 1;
								break;
							case equip_slot::RIGHTRING:
								mx = (*InvRect)[5].X + 1;
								my = (*InvRect)[5].Y - 1;
								break;
							case equip_slot::AMULET:
								mx = (*InvRect)[6].X + 1;
								my = (*InvRect)[6].Y - 1;
								break;
							case equip_slot::LEFTHAND:
								mx = (*InvRect)[7].X + 1; //571;//
								my = (*InvRect)[7].Y - 1; //78;// 
								break;
							case equip_slot::RIGHTHAND:
								mx = (*InvRect)[13].X + 1;//339;// 
								my = (*InvRect)[13].Y - 1;//79;//
								break;
							case equip_slot::BODY:
								mx = (*InvRect)[19].X + 1;//458;//
								my = (*InvRect)[19].Y - 1;//80;// 
								break;
							}
						}
					}
					else if (i < 47) {
						Item temp(&(*player)[my_pnum].InvList[i - 7]);
						if (target == temp) {
							for (int rect_index = 0; rect_index < 40; rect_index++) {
								if ((*player)[*myplr].InvGrid[rect_index] == i - 6) {
									int index = rect_index + 25;
									mx = (*InvRect)[index].X + 1; //338 + (rect_index % 10) * 30; // //338
									my = (*InvRect)[index].Y - 1; //224 + (rect_index / 10) * 30;// //224
									break;
								}
							}
							break;
						}
					}
					else {
						Item temp(&(*player)[my_pnum].SpdList[i - 47]);
						if (target == temp)
						{
							int index = 18 + i;
							mx = 210 + (i - 47) * 30;//(*InvRect)[index].X + 10;
							my = 370;//(*InvRect)[index].Y - 10;
							break;
						}
					}
				}
				if (mx != 0 && my != 0) {
					CheckInvCut(*myplr, mx, my);
				}
			}
			/*static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
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
			return false;*/
		}
		
		//If the cursor is the repair icon, this will simulate left clicking the item in your inventory
		//or equip location.
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

		//Selects the store option passed to it, assuming the option exists.
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

		//DO NOT USE.
		bool setRightClickSpell(spell_id id)
		{
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
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

		//Sets the right click spell to the spell, and spell type, passed to the function. Use spell
		//type to indicate if you're setting it as a charge, scroll, or known spell. If this is not
		//set correctly, you could end up casting the null spell.
		bool setRightClickSpell(spell_id id, spell_type type)
		{
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto SetSpell = reinterpret_cast<void(__cdecl *)()>(0x403F69);
			auto pSpell = reinterpret_cast<int(*)>(0x4B8834);
			auto pSplType = reinterpret_cast<int(*)>(0x4B8954);
			*pSpell = static_cast<int>(id);
			*pSplType = static_cast<int>(type);
			SetSpell();
			return true;
		}

		//Issues an attack command at the x, y coordinates indicated by the Point passed to it.
		//This simulates shift left clicking at that point.
		void shiftAttack(Point target) {
			auto NetSendCmdLoc = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (static_cast<item_type>((*player)[*myplr].InvBody[4]._itype) == item_type::ITYPE_BOW || static_cast<item_type>((*player)[*myplr].InvBody[5]._itype) == item_type::ITYPE_BOW)
				NetSendCmdLoc(1u, static_cast<unsigned char>(_cmd_id::CMD_RATTACKXY), target.x, target.y);
			else
				NetSendCmdLoc(1u, static_cast<unsigned char>(_cmd_id::CMD_SATTACKXY), target.x, target.y);
		}

		//Returns the number of stat points the character has available to spend.
		int statPoints() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum]._pStatPts; }

		//Issues a talk command targetting the towner passed to the function. Simulates left clicking
		//with a towner selected.
		void talkToTowner(Towner towner) {
			static auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall *)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
			if (okToAct()) {
				if (-1 < towner.id() && towner.id() < 16)
					NetSendCmdLocParam1(1u, static_cast<unsigned char>(_cmd_id::CMD_TALKXY), towner.x(), towner.y(), towner.id());
			}
		}

		//Returns the walk path generated by Diablo for the character. The walk path does not contain
		//coordinates, but instead contains the directions the character should step.
		char* walkPath() { static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448); return (*player)[my_pnum].walkpath; }

		//Issues a walk command to the x and y coordinates passed to the function. Simulates a left click
		//on that tile with nothing selected.
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

		//Uses the item passed to the function, assuming it is in the character's inventory or belt.
		void useItem(Item target)
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto UseInvItem = reinterpret_cast<int(__fastcall *)(int pnum, int cii)>(0x41ED29);
			int inv_index = target.inventoryIndex(my_pnum);
			if (inv_index != -1)
				UseInvItem(my_pnum, inv_index);
		}

		//Uses the item at the equip_slot passed to the function.
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

		//DO NOT USE
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