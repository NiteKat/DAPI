#pragma once
#include"Structs.h"
#include<string>

namespace DAPI
{
	struct Item
	{
		Item() { my_item = nullptr; }

		Item(ItemStruct* new_item) {
			my_item = new_item;
		}

		//Compares if the two Item objects are pointing to the same spot in memory.
		bool operator==(const Item& other) const {
			return (this->my_item) == (other.my_item);
		}

		//Compares if the two Item objects are pointing to different spots in memory.
		bool operator!=(const Item& other) {
			return (this->my_item) != (other.my_item);
		}
		
		//Returns the armor class of the item.
		int armorClass() {
			if (my_item)
				return my_item->_iAC;
			else
				return -1;
		}

		//Returns the current number of charges the item has.
		int charges() {
			if (my_item)
				return my_item->_iCharges;
			else
				return -1;
		}

		//Returns the iLoc flag of the item, which can be used to determine if an item
		//is one handed or two handed for example.
		item_equip_type equipType() {
			if (my_item)
				return static_cast<item_equip_type>(my_item->_iLoc);
			else
				return item_equip_type::ILOC_INVALID;
		}

		//Returns what the cursor/graphic is for the item.
		item_class itemClass() {
			if (my_item)
				return static_cast<item_class>(my_item->_iCurs);
			else
				return item_class::NONE;
		}

		//Returns the current durability of the item.
		int currentDurability() {
			if (my_item)
				return my_item->_iDurability;
			else
				return -1;
		}

		//If the item is on the ground, this will return the index in the items array
		//where the item is stored.
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

		//Returns if the item is identified. If it is non-magical, it will return false.
		bool identified() {
			if (my_item)
				return static_cast<bool>(my_item->_iMagical) && static_cast<bool>(my_item->_iIdentified);
			else
				return false;
		}

		//If the item is in equipped, in your inventory, or in your belt then this returns
		//the value that represents the slot it is in. You can use the equip_slot enum
		//to translate where the item is.
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

		//Returns true if this Item's pointer is not null.
		bool isValid() {
			if (my_item)
				return true;
			return false;
		}

		//If the item is on the ground, returns if the tile it is located at is visible.
		bool isVisible() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			if (my_item)
				return (*dFlags)[my_item->_ix][my_item->_iy] & 0x40;
			return false;
		}

		//Returns true if the item is magical.
		bool magical() {
			if (my_item)
				return static_cast<bool>(my_item->_iMagical);
			else
				return false;
		}

		//Returns the maximum number of charges the item can have.
		int maxCharges() {
			if (my_item)
				return my_item->_iMaxCharges;
			else
				return -1;
		}

		//Returns the max damage done by the item.
		int maxDamage() {
			if (my_item)
				return my_item->_iMaxDam;
			else
				return -1;
		}

		//Returns the minimum damage done by the item.
		int minDamage() {
			if (my_item)
				return my_item->_iMinDam;
			else
				return -1;
		}

		//Returns the maximum durability of the item.
		int maxDurability() {
			if (my_item)
				return my_item->_iMaxDur;
			else
				return -1;
		}

		//Returns if the item is flagged as the character meeting the stat requirements to use
		//or equip this item. This is the equivalent of looking at the item and seeing if it
		//is colored red or normally.
		bool meetStatRequirements() {
			if (my_item)
				return static_cast<bool>(my_item->_iStatFlag);
			else
				return false;
		}

		//Returns the misc ID of the item. You can compare this against the item_misc_id enum.
		int miscId()
		{
			if (my_item)
				return my_item->_iMiscId;
			else
				return -1;
		}

		//Returns the name of the item.
		std::string name()
		{
			std::string return_value;
			if (my_item) {
				if (my_item->_iIdentified)
					return_value = my_item->_iIName;
				else
					return_value = my_item->_iName;
			}
			else
				return_value = "";
			return return_value;
		}

		//Returns the + damage modifier of the item.
		int plusDamage() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLDamMod;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the plus damage from enemies modifier.
		int plusDamageFromEnemies() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLGetHit;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Dexterity added by the item.
		int plusDexterity() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLDex;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the Fire Resist percentage added by the item.
		int plusFireResistPercent() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLFR;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Life added by the item.
		int plusLife() {
			if (my_item) {
				if (my_item->_iIdentified)
					return static_cast<int>(floor(my_item->_iPLHP / 64));
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the Lightning Resist percentage added by the item.
		int plusLightningResistPercent() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLLR;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the Light Radius Percentage added by the item.
		int plusLightRadiusPercent() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLLight;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Magic added by the item.
		int plusMagic() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLMag;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the Magic Resist percentage added by the item.
		int plusMagicResistPercent() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLMR;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Mana added by the item.
		int plusMana() {
			if (my_item) {
				if (my_item->_iIdentified)
					return static_cast<int>(floor(my_item->_iPLMana / 64));
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns as an integer number the percentage this item's Armor Class is increased.
		int plusPercentArmorClass() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iAC;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the percent damage increase this item provides.
		int plusPercentDamage() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLDam;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the number by which this item increases spell levels.
		int plusSpellLevel() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iSplLvlAdd;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Strength added by this item.
		int plusStrength() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLStr;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the amount of Vitality added by this item.
		int plusVitality() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLVit;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the additional To Hit provided by this item as an intenger.
		int plusToHit() {
			if (my_item) {
				if (my_item->_iIdentified)
					return my_item->_iPLToHit;
				else
					return 0;
			}
			else
				return 0;
		}

		//Returns the Dexterity required to equip or use the item.
		int requiredDexterity() {
			if (my_item)
				return my_item->_iMinDex;
			else
				return -1;
		}

		//Returns the Magic required to equip or use the item.
		int requiredMagic() {
			if (my_item)
				return my_item->_iMinMag;
		}

		//Returns the Strength required to equip or use the item.
		int requiredStrength() {
			if (my_item)
				return my_item->_iMinStr;
			else
				return -1;
		}

		//Returns the spell associated with this item. For example, if this is a staff with
		//charges, it returns the spell the staff allows you to cast. If this item is a scroll,
		//it returns the spell cast upon using the item.
		spell_id spell() {
			if (my_item)
				return static_cast<spell_id>(my_item->_iSpell);
			else
				return spell_id::SPL_NULL;
		}

		//Returns the type of the item.
		int type() {
			if (my_item)
				return my_item->_itype;
			else
				return -1;
		}

		//If the item is a unique item, returns which unique item it is.
		_unique_items uniqueId() {
			if (my_item) {
				if (my_item->_iIdentified)
					return static_cast<_unique_items>(my_item->_iUid);
				else
					return _unique_items::UITEM_CLEAVER;
			}
			else
				return _unique_items::UITEM_CLEAVER;
		}

		//Returns the value associated with this item. If it is in a buy menu, this will be the
		//price of purchasing the item. If it is in a sell menu, this will be the amount for
		//which the item sells.
		int value() {
			return my_item->_ivalue;
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