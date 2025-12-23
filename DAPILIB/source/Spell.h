#pragma once
#include <string>

#include "PlayerData.h"

namespace DAPI
{
  enum struct SpellID
  {
    SPNULL = 0x0,
    FIREBOLT = 0x1,
    HEAL = 0x2,
    LIGHTNING = 0x3,
    FLASH = 0x4,
    IDENTIFY = 0x5,
    FIREWALL = 0x6,
    TOWN = 0x7,
    STONE = 0x8,
    INFRA = 0x9,
    RNDTELEPORT = 0xA,
    MANASHIELD = 0xB,
    FIREBALL = 0xC,
    GUARDIAN = 0xD,
    CHAIN = 0xE,
    WAVE = 0xF,
    DOOMSERP = 0x10,
    BLODRIT = 0x11,
    NOVA = 0x12,
    INVISIBIL = 0x13,
    FLAME = 0x14,
    GOLEM = 0x15,
    BLODBOIL = 0x16,
    TELEPORT = 0x17,
    APOCA = 0x18,
    ETHEREALIZE = 0x19,
    REPAIR = 0x1A,
    RECHARGE = 0x1B,
    DISARM = 0x1C,
    ELEMENT = 0x1D,
    CBOLT = 0x1E,
    HBOLT = 0x1F,
    RESURRECT = 0x20,
    TELEKINESIS = 0x21,
    HEALOTHER = 0x22,
    FLARE = 0x23,
    BONESPIRIT = 0x24,
    INVALID = -1,
  };

  enum struct SpellTypeID
  {
    SKILL = 0x0,
    SPELL = 0x1,
    SCROLL = 0x2,
    CHARGES = 0x3,
    INVALID = 4,
  };

  struct SpellType
  {
    SpellType(SpellTypeID ID) : ID(ID) {}
    SpellTypeID getID() { return ID; }
    std::string getName();
  private:
    SpellTypeID ID;
  };

  struct Spell
  {
    Spell(SpellID ID, SpellTypeID type) : ID(ID), type(type), level(0), playerClass(ClassID::UNKNOWN), playerLevel(0), minDamage(-1), maxDamage(-1) {}
    Spell(SpellID ID, SpellTypeID type, int level, ClassID playerClass, int playerLevel, int magic);
    SpellID getID() { return ID; }
    std::string getName();
    SpellType getType() { return type; }
    int getManaCost();
    int getMinimumDamage() { return minDamage; }
    int getMaximumDamage() { return maxDamage; }
    int getLevel(int splLvlAdd) { return level + splLvlAdd; }
    int getRawManaCost(int splLvlAdd);
  private:
    SpellID ID;
    SpellType type;
    int level;
    ClassID playerClass;
    int playerLevel;
    int minDamage;
    int maxDamage;
  };
}