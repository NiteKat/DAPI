#include "Player.h"

namespace DAPI
{
#define MAXCHARLEVEL			51
  int ExpLvlsTbl[MAXCHARLEVEL] = {
  0,
  2000,
  4620,
  8040,
  12489,
  18258,
  25712,
  35309,
  47622,
  63364,
  83419,
  108879,
  141086,
  181683,
  231075,
  313656,
  424067,
  571190,
  766569,
  1025154,
  1366227,
  1814568,
  2401895,
  3168651,
  4166200,
  5459523,
  7130496,
  9281874,
  12042092,
  15571031,
  20066900,
  25774405,
  32994399,
  42095202,
  53525811,
  67831218,
  85670061,
  107834823,
  135274799,
  169122009,
  210720231,
  261657253,
  323800420,
  399335440,
  490808349,
  601170414,
  733825617,
  892680222,
  1082908612,
  1310707109,
  1583495809
  };

  Player::Player()
  {
    data = std::make_unique<PlayerData>();
  }

  bool Player::canCast(Spell spell)
  {
    if (spell.getType().getID() == DAPI::SpellTypeID::CHARGES &&
        data->InvBody[4] && data->InvBody[4]->getSpell() != spell.getID())
        return false;

    if (spell.getType().getID() == DAPI::SpellTypeID::SPELL &&
      (spell.getLevel() == 0 || data->_pMana < spell.getRawManaCost()))
      return false;

    if (spell.getType().getID() == DAPI::SpellTypeID::SCROLL)
    {
      int i = static_cast<int>(spell.getID()) - 1;
      auto scrollSpells = data->_pScrlSpells;
      scrollSpells = scrollSpells >> i;
      if (!(scrollSpells & 0x1))
        return false;
    }

    return true;
  }

  std::vector<Spell> Player::getAvailableSpells()
  {
    std::vector<Spell> returnValue;
    auto abilitySpells = data->_pAblSpells;
    auto scrollSpells = data->_pScrlSpells;

    for (int i = 0; i < 37; i++)
    {
      if (data->_pSplLvl[i] > 0)
        returnValue.push_back(Spell{ static_cast<SpellID>(i), SpellTypeID::SPELL, data->_pSplLvl[i], this->getClass().getID(), data->_pLevel, data->_pMagic });
      if (abilitySpells & 0x1)
        returnValue.push_back(Spell{ static_cast<SpellID>(i + 1), SpellTypeID::SKILL });
      if (scrollSpells & 0x1)
        returnValue.push_back(Spell{ static_cast<SpellID>(i + 1), SpellTypeID::SCROLL });
      scrollSpells = scrollSpells >> 1;
      abilitySpells = abilitySpells >> 1;
    }

    if (data->InvBody[4] && data->InvBody[4]->getSpell() != SpellID::SPNULL)
      returnValue.push_back(Spell{ data->InvBody[4]->getSpell(), SpellTypeID::CHARGES });
    else if (data->InvBody[5] && data->InvBody[5]->getSpell() != SpellID::SPNULL)
      returnValue.push_back(Spell{ data->InvBody[5]->getSpell(), SpellTypeID::CHARGES });

    return returnValue;
  }
  CharacterClass Player::getClass()
  {
    return CharacterClass{ static_cast<ClassID>(data->_pClass) };
  }
  int Player::getExperienceToNextLevel()
  {
    return ExpLvlsTbl[data->_pLevel];
  }

  int Player::getMinimumDamage()
  {
    int mindam = data->_pIMinDam;
    mindam += data->_pIBonusDam * mindam / 100;
    mindam += data->_pIBonusDamMod;
    if (data->InvBody[static_cast<int>(InvBodyLocation::HAND_LEFT)])
    {
      if (data->InvBody[static_cast<int>(InvBodyLocation::HAND_LEFT)]->getType() == ItemType::BOW) {
        if (static_cast<ClassID>(data->_pClass) == ClassID::ROGUE)
          mindam += data->_pDamageMod;
        else
          mindam += data->_pDamageMod >> 1;
      }
      else {
        mindam += data->_pDamageMod;
      }
    }
    else
      mindam += data->_pDamageMod;
    return mindam;
  }
  int Player::getMaximumDamage()
  {
    int maxdam = data->_pIMaxDam;
    maxdam += data->_pIBonusDam * maxdam / 100;
    maxdam += data->_pIBonusDamMod;
    if (data->InvBody[static_cast<int>(InvBodyLocation::HAND_LEFT)])
    {
      if (data->InvBody[static_cast<int>(InvBodyLocation::HAND_LEFT)]->getType() == ItemType::BOW) {
        if (static_cast<ClassID>(data->_pClass) == ClassID::ROGUE)
          maxdam += data->_pDamageMod;
        else
          maxdam += data->_pDamageMod >> 1;
      }
      else {
        maxdam += data->_pDamageMod;
      }
    }
    else
      maxdam += data->_pDamageMod;
    return maxdam;
  }

  int Player::getMagicResist()
  {
    if (data->_pMagResist < 75)
      return data->_pMagResist;

    return 75;
  }

  int Player::getFireResist()
  {
    if (data->_pFireResist < 75)
      return data->_pFireResist;

    return 75;
  }

  int Player::getLightningResist()
  {
    if (data->_pLightResist < 75)
      return data->_pLightResist;

    return 75;
  }

  std::shared_ptr<Item> Player::getEquippedItem(InvBodyLocation location)
  {
    if (data->InvBody.empty())
      return nullptr;

    return data->InvBody[static_cast<int>(location)];
  }

  std::vector<std::shared_ptr<Item>> Player::getInventoryItems()
  {
    std::vector<std::shared_ptr<Item>> returnValue;
    for (int i = 0; i < 40; i++)
    {
      if (data->InvList[i])
        returnValue.push_back(data->InvList[i]);
    }
    return returnValue;
  }

  std::vector<std::shared_ptr<Item>> Player::getSpeedItems()
  {
    std::vector<std::shared_ptr<Item>> returnValue;
    for (int i = 0; i < 8; i++)
    {
      returnValue.push_back(data->SpdList[i]);
    }
    return returnValue;
  }
}