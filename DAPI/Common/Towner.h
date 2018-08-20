#pragma once
#include"Structs.h"
#include<string>

namespace DAPI
{
	struct Towner {
		Towner() { my_towner = nullptr; }

		Towner(TownerStruct* new_towner) { my_towner = new_towner; }

		int id() {
			static auto towner = reinterpret_cast<TownerStruct(*)[16]>(0x6AAC38);
			for (int i = 0; i < 16; i++)
			{
				if (my_towner == &(*towner)[i])
					return i;
			}
			return -1;
		}

		bool isValid() {
			if (my_towner)
				return true;
			return false;
		}

		std::string name() {
			if (my_towner)
				return my_towner->_tName;
			else
				return "INVALID";
		}

		_talker_id type() {
			if (my_towner)
				return static_cast<_talker_id>(my_towner->_ttype);
			else
				return _talker_id::TOWN_INVALID;
		}

		int x() {
			if (my_towner)
				return my_towner->_tx;
			else
				return -1;
		}

		int y() {
			if (my_towner)
				return my_towner->_ty;
			else
				return -1;
		}

	private:
		TownerStruct * my_towner;
	};
}