#pragma once
#include<string>
#include<map>

#include "Item.h"

namespace DAPI
{
  enum struct PlayerMode {
    STAND = 0,
    WALK = 1,
    WALK2 = 2,
    WALK3 = 3,
    ATTACK = 4,
    RATTACK = 5,
    BLOCK = 6,
    GOTHIT = 7,
    DEATH = 8,
    SPELL = 9,
    NEWLVL = 10,
    QUIT = 11,
  };

  enum struct ClassID
  {
    WARRIOR = 0x0,
    ROGUE = 0x1,
    SORCERER = 0x2,
    NUMCLASSES = 0x3,
    UNKNOWN = 0x4
  };

  struct CharacterClass
  {
    CharacterClass(ClassID ID) : ID(ID) {}
    ClassID getID() { return ID; }
    std::string getName();
  private:
    ClassID ID;
  };

  struct PlayerData
  {
    PlayerMode _pmode;
    int pnum;
    int plrlevel;
    int _px;
    int _py;
    int _pfutx;
    int _pfuty;
    int _pdir;

    int _pRSpell;
    char _pRSplType;

    char _pSplLvl[64];
    unsigned __int64 _pMemSpells;
    unsigned __int64 _pAblSpells;
    unsigned __int64 _pScrlSpells;

    std::string _pName;
    int _pClass;

    int _pStrength;
    int _pBaseStr;
    int _pMagic;
    int _pBaseMag;
    int _pDexterity;
    int _pBaseDex;
    int _pVitality;
    int _pBaseVit;

    int _pStatPts;

    int _pDamageMod;

    int _pHitPoints;
    int _pMaxHP;
    int _pMana;
    int _pMaxMana;
    char _pLevel;
    int _pExperience;

    char _pArmorClass;

    char _pMagResist;
    char _pFireResist;
    char _pLightResist;

    int _pGold;

    std::map<int, std::shared_ptr<Item>> InvBody;
    std::shared_ptr<Item> InvList[40];
    int InvGrid[40];
    std::shared_ptr<Item> SpdList[8];
    std::shared_ptr<Item> HoldItem;

    int _pIMinDam;
    int _pIMaxDam;
    int _pIBonusDam;
    int _pIAC;
    int _pIBonusToHit;
    int _pIBonusAC;
    int _pIBonusDamMod;
    bool pManaShield;
  };
}