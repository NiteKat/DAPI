#pragma once
#include"Structs.h"
#include<string>

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
		bool operator==(const Monster& other) const {
			return (this->my_monster) == (other.my_monster);
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
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D330);
			for (int i = 0; i < 200; i++)
			{
				if (&(*monster)[i] == my_monster)
					return i;
			}
			return -1;
		}

		bool isAlive() {
			if (my_monster && isVisible())
				return 0 < my_monster->_mhitpoints;
			else
				return false;
		}

		bool isValid() {
			if (my_monster)
				return true;
			return false;
		}

		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			return (*dFlags)[my_monster->_mx][my_monster->_my] & 0x40;
		}

		MON_MODE mode() {
			if (my_monster)
				return static_cast<MON_MODE>(my_monster->_mmode);
			else
				return MON_MODE::MM_INVALID;
		}

		std::string name() {
			if (my_monster)
				return static_cast<std::string>(my_monster->mName);
			else
				return static_cast<std::string>("Invalid Monster");
		}

		_monster_id type() {
			if (my_monster)
				return static_cast<_monster_id>(my_monster->MType->mtype);
			else
				return _monster_id::NONE;
		}

		auto x() {
			if (my_monster && isVisible())
				return my_monster->_mx;
			else
				return -1;
		}

		auto y() {
			if (my_monster && isVisible())
				return my_monster->_my;
			else
				return -1;
		}

	private:
		MonsterStruct* my_monster;
	};
}