#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Door
	{
		Door() { my_door = nullptr; }
		Door(ObjectStruct* ndoor) { my_door = ndoor; }

		bool operator==(const Door& other) const {
			return (this->my_door) == (other.my_door);
		}

		int id() {
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 0; i < 200; i++)
			{
				if (&(*object)[i] == my_door)
					return i;
			}
			return -1;
		}

		bool isBlocked() {
			if (my_door->_oVar4 == 2)
				return true;
			return false;
		}


		bool isOpen() {
			if (!my_door->_oVar4)
				return false;
			return true;
		}

		bool isValid() {
			if (my_door)
				return true;
			else
				return false;
		}

		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			return (*dFlags)[my_door->_ox][my_door->_oy] & 0x40;
		}

		int x() {
			if (my_door)
				return my_door->_ox;
			else
				return -1;
		}

		int y() {
			if (my_door)
				return my_door->_oy;
			else
				return -1;
		}

	private:
		ObjectStruct* my_door;
	};
}