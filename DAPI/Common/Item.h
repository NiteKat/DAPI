#pragma once
#include"Structs.h"

namespace DAPI
{
	struct Item
	{
		Item() { my_item = nullptr; }

		Item(ItemStruct* nmy_item) {
			my_item = nmy_item;
		}

		bool operator==(const Item& other) {
			return (this->my_item) == (other.my_item);
		}
		
		int armorClass() {
			if (my_item)
				return my_item->_iAC;
			else
				return -1;
		}

		int groundId() {
			auto item = reinterpret_cast<ItemStruct(*)[127]>(0x635A28);
			auto dItem = reinterpret_cast<char(*)[112][112]>(0x5C9A10);
			if (my_item)
			{
				int index = (*dItem)[my_item->_ix][my_item->_iy];
				if (index)
					return index - 1;
				else
					return -1;
			}
			return -1;
		}

		int inventoryIndex(int pnum) {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			for (int i = 0; i < 7; i++)
			{
				if (&((*player)[pnum].InvBody[i]) == my_item)
					return i;
			}
			for (int i = 0; i < 40; i++)
			{
				if (&((*player)[pnum].InvList[i]) == my_item)
					return i + 7;
			}
			for (int i = 0; i < 8; i++)
			{
				if (&((*player)[pnum].SpdList[i]) == my_item)
					return i + 47;
			}
			return -1;
		}

		bool isValid() {
			if (my_item)
				return true;
			return false;
		}

		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			if (my_item)
				return (*dFlags)[my_item->_ix][my_item->_iy] & 0x40;
			return false;
		}

		int maxDamage() {
			if (my_item)
				return my_item->_iMaxDam;
			else
				return -1;
		}

		int minDamage() {
			if (my_item)
				return my_item->_iMinDam;
			else
				return -1;
		}

		int miscId()
		{
			if (my_item)
				return my_item->_iMiscId;
			else
				return -1;
		}

		int type() {
			if (my_item)
				return my_item->_itype;
			else
				return -1;
		}

		int x() {
			if (my_item)
				return my_item->_ix;
			return -1;
		}

		int y() {
			if (my_item)
				return my_item->_iy;
			return -1;
		}

		struct Compare {
			bool operator () (const Item& a, const Item& b) const {
				return a.my_item < b.my_item;
			}
		};
	private:
		ItemStruct* my_item;
	};
}