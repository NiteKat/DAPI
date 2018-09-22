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

		bool operator==(const Object& other) const {
			return (this->my_object) == (other.my_object);
		}

		//Returns the index at which the object is stored in the object array.
		int id() {
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 0; i < 200; i++)
			{
				if (&(*object)[i] == my_object)
					return i;
			}
			return -1;
		}

		//Returns if the object is solid.
		bool isSolid() { return my_object->_oSolidFlag; }

		//Returns true if the pointer is not null.
		bool isValid() {
			bool valid = my_object;
			return valid;
		}

		//Returns true if the tile on which this object exists is flagged as visible
		//by Diablo.
		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			return (*dFlags)[my_object->_ox][my_object->_oy] & 0x40;
		}

		//Returns true if the object is selectable, meaning if you mouse over it, does it
		//display information in the info box, and can you click it to interact with it.
		bool selectable() {
			return my_object->_oSelFlag;
		}

		//If this is a shrine, it will return the type associated with the shrine. You can
		//use the shrine_type enum to compare.
		shrine_type shrineType() {
			if (my_object && static_cast<_object_id>(my_object->_otype) == _object_id::OBJ_SHRINEL || static_cast<_object_id>(my_object->_otype) == _object_id::OBJ_SHRINER) {
				return static_cast<shrine_type>(my_object->_oVar1);
			}
			else
				return shrine_type::NONE;
		}

		//Returns the type of the object.
		_object_id type() {
			if (my_object)
			{
				switch (static_cast<_object_id>(my_object->_otype))
				{
				case _object_id::OBJ_BARRELEX:
					return _object_id::OBJ_BARREL;
				default:
					return static_cast<_object_id>(my_object->_otype);
				}
			}
		}

		int x() {
			if (my_object)
				return my_object->_ox;
			else
				return -1;
		}

		int y() {
			if (my_object)
				return my_object->_oy;
			else
				return -1;
		}

		struct Compare {
			bool operator () (const Object& a, const Object& b) const {
				return a.my_object < b.my_object;
			}
		};

	private:
		ObjectStruct* my_object;
	};
}