#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Door
	{
		Door() { my_door = nullptr; }
		Door(ObjectStruct* ndoor) { my_door = ndoor; }

		//Compares if the two Door objects are pointing to the same spot in memory.
		bool operator==(const Door& other) const {
			return (this->my_door) == (other.my_door);
		}

		//Returns the index in the object array where this door is located. If it
		//is not found, then -1 is returned.
		int id() {
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 0; i < 200; i++)
			{
				if (&(*object)[i] == my_door)
					return i;
			}
			return -1;
		}

		//Returns if the door is blocked or not.
		bool isBlocked() {
			if (my_door->_oVar4 == 2)
				return true;
			return false;
		}
		
		//Returns if the door is open or not.
		bool isOpen() {
			if (!my_door->_oVar4)
				return false;
			return true;
		}

		//Returns true if the pointer is not null.
		bool isValid() {
			if (my_door)
				return true;
			else
				return false;
		}

		//Returns true if the tile this door is at is flagged as visible by
		//Diablo.
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