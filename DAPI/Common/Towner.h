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

		std::string name() {
			if (my_towner)
				return my_towner->_tName;
			else
				return "INVALID";
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