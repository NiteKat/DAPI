#pragma once
#include"Structs.h"
#include"PlayerCharacter.h"
#include<vector>
#include"Trigger.h"
#include"Object.h"
#include"Monster.h"
#include"Door.h"

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

		auto validObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 1; i < 127; i++)
			{
				bool add_object = false;
				switch ((*object)[i]._otype)
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
					if ((*object)[i]._oSelFlag)
						add_object = true;
					break;
				default:
					break;
				}
				if (add_object)
				{
					Object new_object(&(*object)[i]);
					return_value.push_back(new_object);
				}
			}
			return return_value;
		}

		auto liveMonsters() {
			std::vector<Monster> return_value;
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D24C);
			for (int i = 1; i < 200; i++)
			{
				bool add_monster = false;
				if ((*monster)[i]._mhitpoints > 0)
					add_monster = true;
				if (add_monster)
				{
					Monster new_monster(&(*monster)[i]);
					return_value.push_back(new_monster);
				}
			}
			return return_value;
		}

		auto doors() {
			std::vector<Door> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 1; i < 127; i++)
			{
				bool add_door = false;
				switch ((*object)[i]._otype)
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
					Door new_door(&(*object)[i]);
					return_value.push_back(new_door);
				}
			}
			return return_value;
		}

	private:

	};

	
}