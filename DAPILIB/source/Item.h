#pragma once
#pragma once
#include <memory>

#include "ItemData.h"

namespace DAPI
{
  enum struct SpellID;
  struct Item
  {
    Item();

    int getID() { return data->ID; }
    ItemType getType() { return static_cast<ItemType>(data->_itype); }
    ItemCursorGraphic getCursorGraphic() { return static_cast<ItemCursorGraphic>(data->_iCurs); }

    bool getIsIdentified() { return data->_iIdentified; }
    ItemQuality getItemQuality() { return static_cast<ItemQuality>(data->_iMagical); }
    std::string getName();
    int getValue();
    int getMinimumDamage() { return data->_iMinDam; }
    int getMaximumDamage() { return data->_iMaxDam; }
    int getArmorClass() { return data->_iAC; }
    int getCurrentDurability() { return data->_iDurability; }
    int getMaxDurability() { return data->_iMaxDur; }
    int getRequiredStrength() { return data->_iMinStr; }
    SpellID getSpell() { return static_cast<SpellID>(data->_iSpell); }
    int getRequiredMagic() { return data->_iMinMag; }
    ItemMiscID getMiscID() { return static_cast<ItemMiscID>(data->_iMiscId); }
    int getRequiredDexterity() { return data->_iMinDex; }
    int getToHit() { return data->_iPLToHit; }
    int getDamagePercent() { return data->_iPLDam; }
    int getACPercent() { return data->_iPLAC; }
    int getFireResist() { return data->_iPLFR; }
    int getLightningResist() { return data->_iPLLR; }
    int getMagicResist() { return data->_iPLMR; }
    int getSpellLevelAdd() { return data->_iSplLvlAdd; }
    int getFireHitMinDamage() { return data->_iFMinDam; }
    int getFireHitMaxDamage() { return data->_iFMaxDam; }
    int getLightningHitMinDamage() { return data->_iLMinDam; }
    int getLightningHitMaxDamage() { return data->_iLMaxDam; }
    int getAddedStrength() { return data->_iPLStr; }
    int getAddedMagic() { return data->_iPLMag; }
    int getAddedDexterity() { return data->_iPLDex; }
    int getAddedVitality() { return data->_iPLVit; }
    int getDamageFromEnemies() { return data->_iPLGetHit; }
    int getAddedLife() { return data->_iPLHP >> 6; }
    int getAddedMana() { return data->_iPLMana >> 6; }
    int getAddedLightRadius() { return data->_iPLLight; }
    int getAddedDamage() { return data->_iPLDamMod; }
    ItemSpecialEffect getFasterAttack();
    bool meetRequirements() { return data->_iStatFlag; }

    int getCharges() { return data->_iCharges; }
    int getMaxCharges() { return data->_iMaxCharges; }

    std::string getEffectText();
    std::pair<ItemEffectID, ItemEffectID> getItemEffectIDs() { return std::make_pair<ItemEffectID, ItemEffectID>(static_cast<ItemEffectID>(data->_iPrePower), static_cast<ItemEffectID>(data->_iSufPower)); }

    int getX() { return data->_ix; }
    int getY() { return data->_iy; }

    bool operator== (Item& other) {
      return data->ID == other.getID();
    }



    std::shared_ptr<ItemData> data;
  };
}