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

		int id() {
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			for (int i = 0; i < 200; i++)
			{
				if (&(*object)[i] == my_object)
					return i;
			}
			return -1;
		}

		bool isSolid() { return my_object->_oSolidFlag; }

		bool isValid() {
			bool valid = my_object;
			return valid;
		}

		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			return (*dFlags)[my_object->_ox][my_object->_oy] & 0x40;
		}

		bool selectable() {
			return my_object->_oSelFlag;
		}

		shrine_type shrineType() {
			if (my_object && static_cast<_object_id>(my_object->_otype) == _object_id::OBJ_SHRINEL || static_cast<_object_id>(my_object->_otype) == _object_id::OBJ_SHRINER) {
				return static_cast<shrine_type>(my_object->_oVar1);
			}
			else
				return shrine_type::NONE;
		}

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

		//Need to change x and y to limit to onscreen.
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