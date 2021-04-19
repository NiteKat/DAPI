#include "Spell.h"

namespace DAPI
{
  std::string spellTypeNames[] = { "Ability", "Spell", "Scroll",
  "Charges", "Invalid" };

  int spellManaCost[] = { 0, 6, 5, 10, 30, 13, 28, 35, 60, 40, 12, 33, 16,
  50, 30, 35, 0, 0, 60, 0, 11, 100, 0, 35, 150, 100, 0, 0, 0, 35, 6, 7,
  20, 15, 5, 25, 24 };

  int spellMinMana[] = { 0, 3, 1, 6, 16, 1, 16, 18, 40, 20, 4, 33, 10, 30,
  18, 20, 0, 0, 35, 0, 6, 60, 0, 15, 90, 100, 0, 0, 0, 20, 6, 3, 20, 8, 1,
  14, 12 };

  int spellManaAdj[] = { 0, 1, 3, 1, 2, 2, 2, 3, 3, 5, 2, 0, 1, 2, 1, 3,
  0, 0, 3, 0, 1, 6, 0, 3, 6, 0, 0, 0, 0, 2, 1, 1, 0, 2, 3, 2, 1 };

  std::string spellNames[] = { "Null", "Firebolt", "Healing", "Lightning",
  "Flash", "Identify", "Fire Wall", "Town Portal", "Stone Curse",
  "Infravision", "Phasing", "Mana Shield", "Fireball", "Guardian",
  "Chain Lightning", "Flame Wave", "Doom Serpents", "Blood Ritual",
  "Nova", "Invisibility", "Inferno", "Golem", "Blood Boil", "Teleport",
  "Apocalypse", "Etherealize", "Item Repair", "Staff Recharge",
  "Trap Disarm", "Elemental", "Charged Bolt", "Holy Bolt", "Resurrect",
  "Telekinesis", "Heal Other", "Blood Star", "Bone Spirit", "Invalid" };

  Spell::Spell(SpellID ID, SpellTypeID type, int level, ClassID playerClass, int playerLevel, int magic) :
    ID(ID), type(type), level(level), playerClass(playerClass), playerLevel(playerLevel), minDamage(-1), maxDamage(-1)
  {
    int k, sl;

    sl = level;

    switch (ID) {
    case SpellID::FIREBOLT:
      minDamage = (magic >> 3) + sl + 1;
      maxDamage = (magic >> 3) + sl + 10;
      break;
    case SpellID::HEAL:
      minDamage = -1;
      maxDamage = -1;
      break;
    case SpellID::LIGHTNING:
      minDamage = 2;
      maxDamage = playerLevel + 2;
      break;
    case SpellID::FLASH:
      minDamage = playerLevel;
      for (k = 0; k < sl; k++) {
        minDamage += minDamage >> 3;
      }
      minDamage += minDamage >> 1;
      maxDamage = minDamage * 2;
      break;
    case SpellID::IDENTIFY:
    case SpellID::TOWN:
    case SpellID::STONE:
    case SpellID::INFRA:
    case SpellID::RNDTELEPORT:
    case SpellID::MANASHIELD:
    case SpellID::DOOMSERP:
    case SpellID::BLODRIT:
    case SpellID::INVISIBIL:
    case SpellID::BLODBOIL:
    case SpellID::TELEPORT:
    case SpellID::ETHEREALIZE:
    case SpellID::REPAIR:
    case SpellID::RECHARGE:
    case SpellID::DISARM:
    case SpellID::RESURRECT:
    case SpellID::TELEKINESIS:
    case SpellID::BONESPIRIT:
      minDamage = -1;
      maxDamage = -1;
      break;
    case SpellID::FIREWALL:
      minDamage = (4 * playerLevel + 8) >> 1;
      maxDamage = (4 * playerLevel + 80) >> 1;
      break;
    case SpellID::FIREBALL:
      minDamage = 2 * playerLevel + 4;
      for (k = 0; k < sl; k++) {
        minDamage += minDamage >> 3;
      }
      maxDamage = 2 * playerLevel + 40;
      for (k = 0; k < sl; k++) {
        maxDamage += maxDamage >> 3;
      }
      break;
    case SpellID::GUARDIAN:
      minDamage = (playerLevel >> 1) + 1;
      for (k = 0; k < sl; k++) {
        minDamage += minDamage >> 3;
      }
      maxDamage = (playerLevel >> 1) + 10;
      for (k = 0; k < sl; k++) {
        maxDamage += maxDamage >> 3;
      }
      break;
    case SpellID::CHAIN:
      minDamage = 4;
      maxDamage = 2 * playerLevel + 4;
      break;
    case SpellID::WAVE:
      minDamage = 6 * (playerLevel + 1);
      maxDamage = 6 * (playerLevel + 10);
      break;
    case SpellID::NOVA:
      minDamage = (playerLevel + 5) >> 1;
      for (k = 0; k < sl; k++) {
        minDamage += minDamage >> 3;
      }
      minDamage *= 5;
      maxDamage = (playerLevel + 30) >> 1;
      for (k = 0; k < sl; k++) {
        maxDamage += maxDamage >> 3;
      }
      maxDamage *= 5;
      break;
    case SpellID::FLAME:
      minDamage = 3;
      maxDamage = playerLevel + 4;
      maxDamage += maxDamage >> 1;
      break;
    case SpellID::GOLEM:
      minDamage = 11;
      maxDamage = 17;
      break;
    case SpellID::APOCA:
      minDamage = 0;
      for (k = 0; k < playerLevel; k++) {
        minDamage += 1;
      }
      maxDamage = 0;
      for (k = 0; k < playerLevel; k++) {
        maxDamage += 6;
      }
      break;
    case SpellID::ELEMENT:
      minDamage = 2 * playerLevel + 4;
      for (k = 0; k < sl; k++) {
        minDamage += minDamage >> 3;
      }
      maxDamage = 2 * playerLevel + 40;
      for (k = 0; k < sl; k++) {
        maxDamage += maxDamage >> 3;
      }
      break;
    case SpellID::CBOLT:
      minDamage = 1;
      maxDamage = (magic >> 2) + 1;
      break;
    case SpellID::HBOLT:
      minDamage = playerLevel + 9;
      maxDamage = playerLevel + 18;
      break;
    case SpellID::HEALOTHER:
      minDamage = -1;
      maxDamage = -1;
      break;
    case SpellID::FLARE:
      minDamage = (magic >> 1) + 3 * sl - (magic >> 3);
      maxDamage = minDamage;
      break;
    }
  }

  std::string SpellType::getName()
  {
    return spellTypeNames[static_cast<int>(ID)];
  }

  std::string Spell::getName()
  {
    if (ID == SpellID::INVALID)
      return spellNames[37];

    return spellNames[static_cast<int>(ID)];
  }

  int Spell::getManaCost()
  {
    if (type.getID() != SpellTypeID::SPELL)
      return 0;

    int ma; // mana amount

    // mana adjust
    int adj = 0;

    // spell level
    int sl = level - 1;

    if (sl < 0) {
      sl = 0;
    }

    if (sl > 0) {
      adj = sl * spellManaAdj[static_cast<int>(ID)];
    }
    if (ID == SpellID::FIREBOLT) {
      adj >>= 1;
    }
    if (ID == SpellID::RESURRECT && sl > 0) {
      adj = sl * (spellManaCost[static_cast<int>(SpellID::RESURRECT)] / 8);
    }

    /*if (spelldata[sn].sManaCost == 255) {
      ma = ((BYTE)plr[id]._pMaxManaBase - adj);
    }*/
    //else {
      ma = (spellManaCost[static_cast<int>(ID)] - adj);
    //}

    ma <<= 6;

    if (ID == SpellID::HEAL) {
      ma = (spellManaCost[static_cast<int>(SpellID::HEAL)] + 2 * playerLevel - adj) << 6;
    }
    if (ID == SpellID::HEALOTHER) {
      ma = (spellManaCost[static_cast<int>(SpellID::HEAL)] + 2 * playerLevel - adj) << 6;
    }

    if (playerClass == ClassID::ROGUE) {
      ma -= ma >> 2;
    }

    if (spellMinMana[static_cast<int>(ID)] > ma >> 6) {
      ma = spellMinMana[static_cast<int>(ID)] << 6;
    }

    return ma >> 6;
  }

  int Spell::getRawManaCost()
  {
    if (type.getID() != SpellTypeID::SPELL)
      return 0;

    int ma; // mana amount

    // mana adjust
    int adj = 0;

    // spell level
    int sl = level - 1;

    if (sl < 0) {
      sl = 0;
    }

    if (sl > 0) {
      adj = sl * spellManaAdj[static_cast<int>(ID)];
    }
    if (ID == SpellID::FIREBOLT) {
      adj >>= 1;
    }
    if (ID == SpellID::RESURRECT && sl > 0) {
      adj = sl * (spellManaCost[static_cast<int>(SpellID::RESURRECT)] / 8);
    }

    ma = (spellManaCost[static_cast<int>(ID)] - adj);

    ma <<= 6;

    if (ID == SpellID::HEAL) {
      ma = (spellManaCost[static_cast<int>(SpellID::HEAL)] + 2 * playerLevel - adj) << 6;
    }
    if (ID == SpellID::HEALOTHER) {
      ma = (spellManaCost[static_cast<int>(SpellID::HEAL)] + 2 * playerLevel - adj) << 6;
    }

    if (playerClass == ClassID::ROGUE) {
      ma -= ma >> 2;
    }

    if (spellMinMana[static_cast<int>(ID)] > ma >> 6) {
      ma = spellMinMana[static_cast<int>(ID)] << 6;
    }

    return ma;
  }

}