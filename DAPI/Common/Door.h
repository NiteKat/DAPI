#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Door
	{
		Door() { my_door = nullptr; }
		Door(ObjectStruct* ndoor) { my_door = ndoor; }

		bool isOpen() {
			if (!my_door->_oVar4)
				return false;
			return true;
		}

		bool isBlocked() {
			if (my_door->_oVar4 == 2)
				return true;
			return false;
		}

	private:
		ObjectStruct* my_door;
	};
}