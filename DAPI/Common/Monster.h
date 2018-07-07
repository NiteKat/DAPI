#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Monster
	{
		Monster() {
			my_monster = nullptr;
		}
		Monster(MonsterStruct* nmonster) {
			my_monster = nmonster;
		}

		auto futurex() {
			if (my_monster)
				return my_monster->_mfutx;
			return -1;
		}

		auto futurey() {
			if (my_monster)
				return my_monster->_mfuty;
			return -1;
		}

		auto hitPoints() {
			if (my_monster)
				return my_monster->_mhitpoints;
			else
				return -1;
		}

		int id() {
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D24C);
			for (int i = 1; i < 200; i++)
			{
				if (&(*monster)[i] == my_monster)
					return i;
			}
			return -1;
		}

		bool isValid() {
			if (my_monster)
				return true;
			return false;
		}

		auto x() {
			if (my_monster)
				return my_monster->_mx;
			else
				return -1;
		}

		auto y() {
			if (my_monster)
				return my_monster->_my;
			else
				return -1;
		}

	private:
		MonsterStruct* my_monster;
	};
}