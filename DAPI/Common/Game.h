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

namespace DAPI
{
	struct Game
	{
		PlayerCharacter self() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			PlayerCharacter myself(*myplr);
			return myself;
		}

		auto activeTriggers() {
			auto trigflag = reinterpret_cast<int(*)[5]>(0x6ABAC8);
			auto trigs = reinterpret_cast<TriggerStruct(*)[5]>(0x6ABAE0);
			std::vector<Trigger> return_value;
			for (int loop = 0; loop < (*trigflag)[4]; loop++)
			{
				Trigger new_trigger(trigs[loop]);
				return_value.push_back(new_trigger);
			}
			return return_value;

		}

		bool isSolid(DAPI::Point point) {
			auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
			static auto nSolidTable = reinterpret_cast<char(*)[2049]>(0x5BB2F0);
			if (!(*dPiece)[point.x][point.y] || (*nSolidTable)[(*dPiece)[point.x][point.y]])
				return true;
			else
				return false;
		}

		auto validObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			for (int i = 1; i < 127; i++)
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

		auto liveMonsters() {
			std::vector<Monster> return_value;
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D24C);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			for (int i = 1; i < 200; i++)
			{
				bool add_monster = false;
				if ((*monster)[i]._mhitpoints > 0)
					add_monster = true;
				if (add_monster)
				{
					int mx = (*monster)[i]._mx;
					int my = (*monster)[i]._my;
					if ((*dFlags)[mx][my] & 0x40)
					{
						Monster new_monster(&(*monster)[i]);
						return_value.push_back(new_monster);
					}
				}
			}
			return return_value;
		}

		auto doors() {
			std::vector<Door> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			for (int i = 1; i < 127; i++)
			{
				bool add_door = false;
				switch (static_cast<_object_id>((*object)[i]._otype))
				{
				case _object_id::OBJ_L1LDOOR:
				case _object_id::OBJ_L1RDOOR:
				case _object_id::OBJ_L2LDOOR:
				case _object_id::OBJ_L2RDOOR:
				case _object_id::OBJ_L3LDOOR:
				case _object_id::OBJ_L3RDOOR:
					add_door = true;
					break;
				default:
					break;
				}
				if (add_door)
				{
					int dx = (*object)[i]._ox;
					int dy = (*object)[i]._oy;
					if ((*dFlags)[dx][dy] & 0x40)
					{
						Door new_door(&(*object)[i]);
						return_value.push_back(new_door);
					}
				}
			}
			return return_value;
		}

		auto groundItems() {
			std::vector<Item> return_value;
			auto item = reinterpret_cast<ItemStruct(*)[127]>(0x635A28);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			auto dItem = reinterpret_cast<char(*)[112][112]>(0x5C9A10);
			for (int i = 0; i < 127; i++)
			{
				bool add_item = false;
				int ix = (*item)[i]._ix;
				int iy = (*item)[i]._iy;
				if ((*dItem)[ix][iy] == i + 1)
				{
					if ((*dFlags)[ix][iy] & 0x40)
					{
						Item new_item(&(*item)[i]);
						return_value.push_back(new_item);
					}
				}
			}
			return return_value;
		}
	private:

	};

	
}