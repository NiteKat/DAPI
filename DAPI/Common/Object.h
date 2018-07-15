#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Object
	{
		Object() {
			my_object = nullptr;
		}
		Object(ObjectStruct* nObject) {
			my_object = nObject;
		}

		int id() {
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 1; i < 200; i++)
			{
				if (&(*object)[i] == my_object)
					return i;
			}
			return -1;
		}

		bool isSolid() { return my_object->_oSolidFlag; }

		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			return (*dFlags)[my_object->_ox][my_object->_oy] & 0x40;
		}

		int x() {
			if (my_object && isVisible())
				return my_object->_ox;
			else
				return -1;
		}

		int y() {
			if (my_object && isVisible())
				return my_object->_oy;
			else
				return -1;
		}
	private:
		ObjectStruct* my_object;
	};
}