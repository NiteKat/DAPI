#pragma once
#include <memory>
#include <vector>

#include "PlayerData.h"
#include "Spell.h"

namespace DAPI
{
  enum struct InvBodyLocation {
    HEAD = 0,
    RING_LEFT = 1,
    RING_RIGHT = 2,
    AMULET = 3,
    HAND_LEFT = 4,
    HAND_RIGHT = 5,
    CHEST = 6,
    NUM_INVLOC,
  };

  struct Player
  {
    Player();
    int getID() { return data->pnum; }
    int getX() { return data->_px; }
    int getY() { return data->_py; }
    int getFutureX() { return data->_pfutx; }
    int getFutureY() { return data->_pfuty; }
    int getDirection() { return data->_pdir; }
    Spell getRightClickSpell() { return Spell{ static_cast<SpellID>(data->_pRSpell), static_cast<SpellTypeID>(data->_pRSplType) }; }
    int getSpellLevel(Spell spell) { return data->_pSplLvl[static_cast<int>(spell.getID())]; }
    int getSpellLevel(SpellID spellID) { return data->_pSplLvl[static_cast<int>(spellID)]; }
    CharacterClass getClass();
    std::string getName() { return data->_pName; }
    int getLife() { return data->_pHitPoints >> 6; }
    int getLevel() { return data->_pLevel; }
    int getExperience() { return data->_pExperience; }
    int getExperienceToNextLevel();
    int getMana() { return data->_pMana >> 6; }
    int getBaseStrength() { return data->_pBaseStr; }
    int getStrength() { return data->_pStrength; }
    int getGold() { return data->_pGold; }
    std::vector<Spell> getAvailableSpells();
    int getBaseMagic() { return data->_pBaseMag; }
    int getMagic() { return data->_pMagic; }
    int getArmorClass() { return data->_pIBonusAC + data->_pIAC + data->_pDexterity / 5; }
    int getBaseDexterity() { return data->_pBaseDex; }
    int getDexterity() { return data->_pDexterity; }
    int getBaseVitality() { return data->_pBaseVit; }
    int getVitality() { return data->_pVitality; }
    int getToHit() { return (data->_pDexterity >> 1) + data->_pIBonusToHit + 50; }
    int getMinimumDamage();
    int getMaximumDamage();
    int getStatPoints() { return data->_pStatPts; }
    int getMagicResist();
    int getFireResist();
    int getLightningResist();
    int getMaxLife() { return data->_pMaxHP >> 6; }
    int getMaxMana() { return data->_pMaxMana >> 6; }
    int* getInventoryGrid() { return data->InvGrid; }
    PlayerMode getMode() { return data->_pmode; }
    std::shared_ptr<Item> getEquippedItem(InvBodyLocation location);
    std::vector<std::shared_ptr<Item>> getInventoryItems();
    std::vector<std::shared_ptr<Item>> getSpeedItems();
    std::shared_ptr<Item> getCursorItem() { return data->HoldItem; }

    std::shared_ptr<PlayerData> data;
  };
}