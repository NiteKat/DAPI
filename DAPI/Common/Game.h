#pragma once
#include"Structs.h"
#include"PlayerCharacter.h"
#include<vector>
#include"Trigger.h"
#include"Object.h"
#include"Monster.h"
#include"Door.h"
#include"Item.h"
#include"Point.h"
#include<queue>
#include"DrawRequest.h"
#include<variant>
#include"Portal.h"



namespace DAPI
{
	template<class... Ts> struct Overload : Ts... {using Ts::operator() ...; };
	template<class...Ts> Overload(Ts...)->Overload<Ts...>;

	struct Game
	{
		//Returns a vectors of all triggers on the current floor that are active.
		auto activeTriggers() {
			auto trigflag = reinterpret_cast<int(*)[5]>(0x6ABAC8);
			auto trigs = reinterpret_cast<TriggerStruct(*)[5]>(0x6ABAE0);
			std::vector<Trigger> return_value;
			for (int loop = 0; loop < (*trigflag)[4]; loop++)
			{
				Trigger new_trigger(&(*trigs)[loop]);
				return_value.push_back(new_trigger);
			}
			return return_value;

		}

		//If you are in one of the shop item lists, or the smith repair screen,
		//then the item passed to this function will be purchased or repaired.
		bool buyItem(Item& item) {
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			auto stextsval = reinterpret_cast<int*>(0x6A8A38);
			auto stextup = reinterpret_cast<int*>(0x69F108);
			auto stextsel = reinterpret_cast<int*>(0x6A8A28);
			auto stextsmax = reinterpret_cast<int*>(0x6A09E4);
			auto STextDown = reinterpret_cast<void(__cdecl *)()>(0x45A757);
			auto STextEnter = reinterpret_cast<void(__cdecl *)()>(0x45BF34);
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			if (static_cast<talk_id>(*stextflag) == talk_id::STORE_SBUY ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_WBUY ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_HBUY ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_BBOY ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_SPBUY ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_SREPAIR)
			{
				bool found_item = false;
				do {
					for (int i = 0; i < 24; i++) {
						if ((*stext)[i]._ssel) {
							std::string item_name = item.name();
							std::string option_name = (*stext)[i]._sstr;
							int idx = *stextsval + ((i - *stextup) >> 2);
							if (option_name == item_name) {
								//item found.
								found_item = true;
								*stextsel = i;
							}
						}
					}
					if (found_item)
						break;
					//if not found, scroll down if possible.
					if (!found_item && *stextsval < *stextsmax) {
						if (!(*stextsval)) {
							//Haven't scrolled yet, need to move to scroll position.
							for (int i = 3; 0 < i; i--) {
								STextDown();
							}
						}
						//Scroll down one.
						STextDown();
					}
					else if (*stextsval == *stextsmax) //Cannot scroll anymore and item not found.
						break;
				} while (true);
				if (!found_item)
					return false;
				//found item, move to confirm screen.
				STextEnter();
				STextEnter();
				return true;
			}
			return false;
		}

		//Returns the icon used for the cursor. You can use the enum cursor_id
		//to determine what the cursor is. If the cursor is CURSOR_FIRSTITEM or
		//greater, then the cursor is currently an item, and you can determine what it
		//is using enum item_class plus CURSOR_FIRSTITEM.
		cursor_id cursor() {
			static auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			return static_cast<cursor_id>(*pcurs);
		}

		//Returns a vector of all doors that are on screen.
		auto doors() {
			std::vector<Door> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dObject = reinterpret_cast<char(*)[112][112]>(0x539C48);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dObject)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						switch (static_cast<_object_id>((*object)[index - 1]._otype))
						{
						case _object_id::OBJ_L1LDOOR:
						case _object_id::OBJ_L1RDOOR:
						case _object_id::OBJ_L2LDOOR:
						case _object_id::OBJ_L2RDOOR:
						case _object_id::OBJ_L3LDOOR:
						case _object_id::OBJ_L3RDOOR:
							return_value.push_back(DAPI::Door(&(*object)[index - 1]));
						default:
							break;
						}
					}
				}
			}
			return return_value;
		}

		//Simulates pressing the Escape key in a store menu.
		bool exitStore() {
			auto STextEsc = reinterpret_cast<void(__cdecl *)()>(0x45A584);
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			if (*stextflag) {
				STextEsc();
				return true;
			}
			return false;
		}

		//Returns all portal slots.
		std::vector<Portal> getPortals() {
			auto portal = reinterpret_cast<PortalStruct(*)[4]>(0x69BC98);
			std::vector<Portal> return_value;
			for (int i = 0; i < 4; i++) {
				return_value.push_back(Portal(&(*portal)[i]));
			}
			return return_value;
		}
		
		//Returns a vector of all objects, excluding doors, that are on screen.
		auto getOnScreenObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dObject = reinterpret_cast<char(*)[112][112]>(0x539C48);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dObject)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						switch (static_cast<_object_id>((*object)[index - 1]._otype))
						{
						case _object_id::OBJ_L1LDOOR:
						case _object_id::OBJ_L1RDOOR:
						case _object_id::OBJ_L2LDOOR:
						case _object_id::OBJ_L2RDOOR:
						case _object_id::OBJ_L3LDOOR:
						case _object_id::OBJ_L3RDOOR:
							break;
						default:
							DAPI::Object new_object(&(*object)[index - 1]);
							return_value.push_back(new_object);
						}
					}
				}
			}
			return return_value;
		}
		
		//Returns the x, y pair of all tiles that are on screen.
		std::vector<Point> getOnScreenPoints() {
			auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			std::vector<DAPI::Point> return_value;
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					DAPI::Point new_point{ (*player)[*myplr].WorldX + dx + cur_cell, (*player)[*myplr].WorldY + dy + cur_cell };
					return_value.push_back(new_point);
				}
			}
			return return_value;
		}

		//Returns all items that are on the ground on screen.
		auto groundItems() {
			std::vector<Item> return_value;
			auto item = reinterpret_cast<ItemStruct(*)[127]>(0x635A28);
			auto dItem = reinterpret_cast<char(*)[112][112]>(0x5C9A10);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dItem)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						Item new_item(&(*item)[index - 1]);
						return_value.push_back(new_item);
					}
				}
			}
			return return_value;
		}

		//Identifies the item passed to the function if the Identify Items menu
		//with Cain is open.
		bool identifyItem(Item item) {
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			auto stextsval = reinterpret_cast<int*>(0x6A8A38);
			auto stextup = reinterpret_cast<int*>(0x69F108);
			auto stextsel = reinterpret_cast<int*>(0x6A8A28);
			auto stextsmax = reinterpret_cast<int*>(0x6A09E4);
			auto STextDown = reinterpret_cast<void(__cdecl *)()>(0x45A757);
			auto STextEnter = reinterpret_cast<void(*)()>(0x45BF34);
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			bool found_item = false;
			if (static_cast<talk_id>(*stextflag) == talk_id::STORE_SIDENTIFY)
			{
				do {
					for (int i = 0; i < 24; i++) {
						if ((*stext)[i]._ssel) {
							std::string item_name = item.name();
							std::string option_name = (*stext)[i]._sstr;
							int idx = *stextsval + ((i - *stextup) >> 2);
							if (option_name == item_name) {
								//item found.
								found_item = true;
								*stextsel = i;
							}
						}
					}
					if (found_item)
						break;
					//if not found, scroll down if possible.
					if (!found_item && *stextsval < *stextsmax) {
						if (!(*stextsval)) {
							//Haven't scrolled yet, need to move to scroll position.
							for (int i = 3; 0 < i; i--) {
								STextDown();
							}
						}
						//Scroll down one.
						STextDown();
					}
					else if (*stextsval == *stextsmax) //Cannot scroll anymore and item not found.
						break;
				} while (true);
				if (!found_item)
					return false;
				//found item, move to confirm screen.
				STextEnter();
				STextEnter();
				STextEnter();
				return true;
			}
			return false;
		}

		//Returns true if the inventory screen is open.
		bool isInventoryOpen() {
			static auto invflag = reinterpret_cast<int*>(0x634CB8);
			return invflag;
		}

		//Returns true if the x, y coordinates contained in point are flagged as
		//solid.
		bool isSolid(Point point) {
			auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
			static auto nSolidTable = reinterpret_cast<char(*)[2049]>(0x5BB2F0);
			if (!(*dPiece)[point.x][point.y] || (*nSolidTable)[(*dPiece)[point.x][point.y]])
				return true;
			else
				return false;
		}

		//Returns a vector of all monsters that are alive, on screen, and the tile
		//they are standing on is flagged as visible.
		auto liveMonsters() {
			std::vector<Monster> return_value;
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D330);
			static auto dMonster = reinterpret_cast<int(*)[112][112]>(0x52D208);
			static auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			auto nummonsters = reinterpret_cast<int(*)>(0x64D328);
			auto monstactive = reinterpret_cast<int(*)[200]>(0x64D008);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dMonster)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						for (int j = 0; j < *nummonsters; j++) {
							if ((*monstactive)[j] == index - 1) {
								if ((*dFlags)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell] & 0x40 &&
									(*monster)[index - 1]._mhitpoints > 0)
								{
									DAPI::Monster new_monster(&(*monster)[index - 1]);
									return_value.push_back(new_monster);
								}
							}
						}
					}
				}
			}
			return return_value;
		}

		//Returns the inventory item being referenced by sell_item when a store's
		//sell menu is open.
		Item matchingInventoryItem(Item& sell_item) {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			auto storehold = reinterpret_cast<ItemStruct(*)[48]>(0x6A09F0);
			auto storehidx = reinterpret_cast<char(*)[48]>(0x6A89F0);
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (static_cast<talk_id>(*stextflag) == talk_id::STORE_SSELL ||
				static_cast<talk_id>(*stextflag) == talk_id::STORE_WSELL) {
				for (int i = 0; i < 48; i++) {
					if (Item(&(*storehold)[i]) == sell_item) {
						char index = (*storehidx)[i];
						if (index < 0) {
							return Item(&(*player)[*myplr].SpdList[~index]);
						}
						else {
							return Item(&(*player)[*myplr].InvList[index]);
						}
					}
				}
			}
			return Item();
		}

		//Returns the items listed in the store menu that is open.
		std::vector<Item> openStoreItems() {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			auto smithitem = reinterpret_cast<ItemStruct(*)[20]>(0x6A8A40);
			auto witchitem = reinterpret_cast<ItemStruct(*)[20]>(0x6A4EF8);
			auto healitem = reinterpret_cast<ItemStruct(*)[20]>(0x6A6BC0);
			auto premiumitem = reinterpret_cast<ItemStruct(*)[20]>(0x69F290);
			auto storehold = reinterpret_cast<ItemStruct(*)[48]>(0x6A09F0);
			std::vector<Item> return_value;
			switch (static_cast<talk_id>(*stextflag)) {
			case talk_id::STORE_SBUY:
				for (auto& item : *smithitem) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			case talk_id::STORE_SPBUY:
				for (auto& item : *premiumitem) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			case talk_id::STORE_SIDENTIFY:
			case talk_id::STORE_SREPAIR:
			case talk_id::STORE_WRECHARGE:
				for (auto& item : *storehold) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			case talk_id::STORE_WBUY:
				for (auto& item : *witchitem) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			case talk_id::STORE_HBUY:
				for (auto& item : *healitem) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			case talk_id::STORE_BBOY:
				break;
			case talk_id::STORE_SSELL:
			case talk_id::STORE_WSELL:
				for (auto& item : *storehold) {
					if (static_cast<item_type>(item._itype) != item_type::ITYPE_NONE)
						return_value.push_back(Item(&item));
					else
						break;
				}
				break;
			default:
				break;
			}
			return return_value;
		}

		//Returns the value of the qTextFlag, which indicates if a towner gossip/quest
		//box is open, or if you are reading one of the books in the dungeon.
		char qTextFlag() {
			auto qtextflag = reinterpret_cast<char*>(0x646d00);
			return *qtextflag;
		}

		//Quits Diablo as if you opened the menu and choose Quit Diablo.
		void quit() {
			auto gamemenu_quit_game = reinterpret_cast<void(*)()>(0x41893B);
			gamemenu_quit_game();
		}

		//If the Recharge staves menu is open, selects the passed item to recharge, if it
		//is listed in the store menu.
		bool rechargeItem(Item& item) {
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			auto stextsval = reinterpret_cast<int*>(0x6A8A38);
			auto stextup = reinterpret_cast<int*>(0x69F108);
			auto stextsel = reinterpret_cast<int*>(0x6A8A28);
			auto stextsmax = reinterpret_cast<int*>(0x6A09E4);
			auto STextDown = reinterpret_cast<void(__cdecl *)()>(0x45A757);
			auto STextEnter = reinterpret_cast<void(__cdecl *)()>(0x45BF34);
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			bool found_item = false;
			if (static_cast<talk_id>(*stextflag) == talk_id::STORE_WRECHARGE)
			{
				do {
					for (int i = 0; i < 24; i++) {
						if ((*stext)[i]._ssel) {
							std::string item_name = item.name();
							std::string option_name = (*stext)[i]._sstr;
							int idx = *stextsval + ((i - *stextup) >> 2);
							if (option_name == item_name) {
								//item found.
								found_item = true;
								*stextsel = i;
							}
						}
					}
					if (found_item)
						break;
					//if not found, scroll down if possible.
					if (!found_item && *stextsval < *stextsmax) {
						if (!(*stextsval)) {
							//Haven't scrolled yet, need to move to scroll position.
							for (int i = 3; 0 < i; i--) {
								STextDown();
							}
						}
						//Scroll down one.
						STextDown();
					}
					else if (*stextsval == *stextsmax) //Cannot scroll anymore and item not found.
						break;
				} while (true);
				if (!found_item)
					return false;
				//found item, move to confirm screen.
				STextEnter();
				STextEnter();
				return true;
			}
			return false;
		}
		
		//If your character is dead, simulates selecting the Restart in Town option from
		//the menu.
		void restartInTown() {
			auto gamemenu_restart_town = reinterpret_cast<void(__cdecl *)()>(0x418A42);
			auto deathflag = reinterpret_cast<int*>(0x69B7B0);
			if (deathflag)
				gamemenu_restart_town();
		}

		//Returns a PlayerCharacter that points at your character.
		PlayerCharacter self() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			PlayerCharacter myself(*myplr);
			return myself;
		}

		//Returns true if playing in Single Player mode.
		bool singlePlayer() {
			auto gbMaxPlayers = reinterpret_cast<char*>(0x679660);
			return *gbMaxPlayers == 1;
		}

		//Sells the passed item if it is listed in the shop's sell menu.
		bool sellItem(Item& item) {
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			auto stextsval = reinterpret_cast<int*>(0x6A8A38);
			auto stextup = reinterpret_cast<int*>(0x69F108);
			auto stextsel = reinterpret_cast<int*>(0x6A8A28);
			auto stextsmax = reinterpret_cast<int*>(0x6A09E4);
			auto STextDown = reinterpret_cast<void(__cdecl *)()>(0x45A757);
			auto STextEnter = reinterpret_cast<void(__cdecl *)()>(0x45BF34);
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			bool found_item = false;
			if (static_cast<talk_id>(*stextflag) == talk_id::STORE_SSELL || static_cast<talk_id>(*stextflag) == talk_id::STORE_WSELL)
			{
				do {
					for (int i = 0; i < 24; i++) {
						if ((*stext)[i]._ssel) {
							std::string item_name = item.name();
							std::string option_name = (*stext)[i]._sstr;
							int idx = *stextsval + ((i - *stextup) >> 2);
							if (option_name == item_name) {
								//item found.
								found_item = true;
								*stextsel = i;
							}
						}
					}
					if (found_item)
						break;
					//if not found, scroll down if possible.
					if (!found_item && *stextsval < *stextsmax) {
						if (!(*stextsval)) {
							//Haven't scrolled yet, need to move to scroll position.
							for (int i = 3; 0 < i; i--) {
								STextDown();
							}
						}
						//Scroll down one.
						STextDown();
					}
					else if (*stextsval == *stextsmax) //Cannot scroll anymore and item not found.
						break;
				} while (true);
				if (!found_item)
					return false;
				//found item, move to confirm screen.
				STextEnter();
				STextEnter();
				return true;
			}
			return false;
		}

		//Opens or closes the inventory screen.
		void toggleInventory() {
			static auto invflag = reinterpret_cast<int*>(0x634CB8);
			if (invflag)
				*invflag = 0;
			else
				*invflag = 1;
		}

		//Returns the talkFlag value, which indicates which towner menu you are currently in.
		talk_id textFlag() {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			return static_cast<talk_id>(*stextflag);
		}

		//Returns a reference to all of the towners if you are in town.
		std::vector<Towner> towners() {
			static auto towner = reinterpret_cast<TownerStruct(*)[16]>(0x6AAC38);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			std::vector<Towner> return_value;
			if ((*player)[*myplr].plrlevel == 0)
			{
				for (int i = 0; i < 16; i++) {
					return_value.push_back(Towner(&(*towner)[i]));
				}
				return return_value;
			}
			else 
				return return_value;
		}

		//If speaking with  a towner, returns the available options you can select.
		std::vector<STextStruct> townerOptions() {
			auto stextflag = reinterpret_cast<char*>(0x6AA705);
			auto stext = reinterpret_cast<STextStruct(*)[24]>(0x69FB40);
			std::vector<STextStruct> return_value;
			if (stextflag) {
				for (auto& item : *stext) {
					if (item._ssel)
						return_value.push_back(item);
				}
			}

			return return_value;
		}

		//Returns barrels, chests, and sarcophagus that are selectable, and whose tiles are
		//flagged as visible.
		auto validObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			for (int i = 0; i < 127; i++)
			{
				bool add_object = false;
				switch (static_cast<_object_id>((*object)[i]._otype))
				{
				case _object_id::OBJ_BARREL:
				case _object_id::OBJ_BARRELEX:
					if ((*object)[i]._oSolidFlag)
						add_object = true;
					break;
				case _object_id::OBJ_CHEST1:
				case _object_id::OBJ_TCHEST1:
				case _object_id::OBJ_CHEST2:
				case _object_id::OBJ_TCHEST2:
				case _object_id::OBJ_CHEST3:
				case _object_id::OBJ_TCHEST3:
				case _object_id::OBJ_SARC:
					if ((*object)[i]._oSelFlag)
						add_object = true;
					break;
				default:
					break;
				}
				if (add_object)
				{
					int ox = (*object)[i]._ox;
					int oy = (*object)[i]._oy;
					if ((*dFlags)[ox][oy] & 0x40)
					{
						Object new_object(&(*object)[i]);
						return_value.push_back(new_object);
					}
				}
			}
			return return_value;
		}

		//Returns all Points where the tiles are flagged as visible.
		auto visiblePoints() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			std::vector<Point> return_value;
			for (int x = 0; x < 112; x++)
			{
				for (int y = 0; y < 112; y++)
				{
					if ((*dFlags)[x][y] & 0x40)
						return_value.push_back(Point{ x, y });
				}
			}
			return return_value;
		}

		//Request that the string parameter be written to the screen where x and y indicate.
		template<typename T>
		void drawTextToScreen(int x, int y, T &&string) {
			draw_queue.emplace_back(StringDrawScreen{ x, y, std::forward<T>(string) });
		}
		
		//Request that the string be written to the map at the tile indicated by x and y.
		template<typename T>
		void drawTextToMap(int x, int y, T &&string) {
			draw_queue.emplace_back(StringDrawMap{ x, y, std::forward<T>(string) });
		}

		//Called by DAPI.dll. Do not call this in your code.
		void onDraw() {
			static auto PrintGameStr = reinterpret_cast<void(__fastcall *)(int x, int y, char *str, int color)>(0x405681);
			static auto dx_lock_mutex = reinterpret_cast<void(__cdecl *)()>(0x41569A);
			static auto dx_unlock_mutex = reinterpret_cast<void(__cdecl *)()>(0x415725);
			static auto ScrollInfo = reinterpret_cast<ScrollStruct*>(0x5BDAF8);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			auto clean = [&] {
				struct Cleanup {
					Cleanup(drawQueueType &queue) : toClean{ queue } {}
					~Cleanup() { toClean.clear(); }
					drawQueueType &toClean;
				};

				return Cleanup(draw_queue);
			}();

			dx_lock_mutex();
			for (auto &drawable : draw_queue) {
				std::visit(Overload{
					[](StringDrawScreen &text) {
					PrintGameStr(text.x, text.y, text.s.data(), 3);
					},
					[](StringDrawMap &text) {
					//Point print_location {tile}
						int row = text.x - player[*myplr]->WorldX;
						int col = text.y - player[*myplr]->WorldY;
						int walkStandX = ScrollInfo->_sxoff;// +plr[myplr]._pyoff;
						int walkStandY = ScrollInfo->_syoff;// +plr[myplr]._pxoff;
						if (player[*myplr]->_pmode == static_cast<int>(player_mode::PLAYER_MODE_WALKING_2) && ScrollInfo->_sdir == 4) {
							walkStandX += 32;
							walkStandY += 16;
						}
						else if (player[*myplr]->_pmode == static_cast<int>(player_mode::PLAYER_MODE_WALKING_2) && ScrollInfo->_sdir == 5) {
							walkStandY += 32;
						}

						else if (player[*myplr]->_pmode == static_cast<int>(player_mode::PLAYER_MODE_WALKING_2) && ScrollInfo->_sdir == 6) {
							walkStandX += -32;
							walkStandY += 16;
						}

						int x2 = 32 * (row - col) + (200 * (walkStandX) / 100 >> 1);
						int y2 = 16 * (row + col) + (200 * (walkStandY) / 100 >> 1) - 16;
						int print_x = x2 + 330;
						int print_y = y2 + 160;
						if (0 < print_x && 0 < print_y && print_x < 640 && print_y < 480)
							PrintGameStr(print_x, print_y, text.s.data(), 3);
					}
				}, drawable);
			}
			dx_unlock_mutex();
		}

	private:
		struct StringDrawScreen {
			int x, y;
			std::string s;
		};

		struct StringDrawMap {
			int x, y;
			std::string s;
		};

		using DrawObject = std::variant<StringDrawScreen,
			StringDrawMap
			// add more here as needed
		>;
		using drawQueueType = std::vector<DrawObject>;
		drawQueueType draw_queue;
		
	};

	
}