#pragma once
#include <string>

namespace DAPI
{
  enum struct ItemQuality {
    NORMAL = 0,
    MAGIC = 1,
    UNIQUE = 2,
  };

  enum struct item_misc_id{
    NONE = 0x0,
    USEFIRST = 0x1,
    FULLHEAL = 0x2,
    HEAL = 0x3,
    OLDHEAL = 0x4,
    DEADHEAL = 0x5,
    MANA = 0x6,
    FULLMANA = 0x7,
    POTEXP = 0x8, /* add experience */
    POTFORG = 0x9, /* remove experience */
    ELIXSTR = 0xA,
    ELIXMAG = 0xB,
    ELIXDEX = 0xC,
    ELIXVIT = 0xD,
    ELIXWEAK = 0xE, /* double check with alpha */
    ELIXDIS = 0xF,
    ELIXCLUM = 0x10,
    ELIXSICK = 0x11,
    REJUV = 0x12,
    FULLREJUV = 0x13,
    USELAST = 0x14,
    SCROLL = 0x15,
    SCROLLT = 0x16,
    STAFF = 0x17,
    BOOK = 0x18,
    RING = 0x19,
    AMULET = 0x1A,
    UNIQUE = 0x1B,
    MEAT = 0x1C, /* from demo/PSX */
    OILFIRST = 0x1D,
    OILOF = 0x1E, /* oils are beta or hellfire only */
    OILACC = 0x1F,
    OILMAST = 0x20,
    OILSHARP = 0x21,
    OILDEATH = 0x22,
    OILSKILL = 0x23,
    OILBSMTH = 0x24,
    OILFORT = 0x25,
    OILPERM = 0x26,
    OILHARD = 0x27,
    OILIMP = 0x28,
    OILLAST = 0x29,
    MAPOFDOOM = 0x2A,
    EAR = 0x2B,
    SPECELIX = 0x2C,
    INVALID = -1,
  };

  enum struct ItemType {
    MISC = 0x0,
    SWORD = 0x1,
    AXE = 0x2,
    BOW = 0x3,
    MACE = 0x4,
    SHIELD = 0x5,
    LARMOR = 0x6,
    HELM = 0x7,
    MARMOR = 0x8,
    HARMOR = 0x9,
    STAFF = 0xA,
    GOLD = 0xB,
    RING = 0xC,
    AMULET = 0xD,
    MEAT = 0xE,
    NONE = -1,
  };

  enum struct ItemClass {
    NONE = 0,
    WEAPON = 1,
    ARMOR = 2,
    MISC = 3,
    GOLD = 4,
    QUEST = 5,
  };

  enum struct ItemMiscID {
    NONE = 0x0,
    USEFIRST = 0x1,
    FULLHEAL = 0x2,
    HEAL = 0x3,
    OLDHEAL = 0x4,
    DEADHEAL = 0x5,
    MANA = 0x6,
    FULLMANA = 0x7,
    POTEXP = 0x8,
    POTFORG = 0x9,
    ELIXSTR = 0xA,
    ELIXMAG = 0xB,
    ELIXDEX = 0xC,
    ELIXVIT = 0xD,
    ELIXWEAK = 0xE,
    ELIXDIS = 0xF,
    ELIXCLUM = 0x10,
    ELIXSICK = 0x11,
    REJUV = 0x12,
    FULLREJUV = 0x13,
    USELAST = 0x14,
    SCROLL = 0x15,
    SCROLLT = 0x16,
    STAFF = 0x17,
    BOOK = 0x18,
    RING = 0x19,
    AMULET = 0x1A,
    UNIQUE = 0x1B,
    MEAT = 0x1C,
    OILFIRST = 0x1D,
    OILOF = 0x1E,
    OILACC = 0x1F,
    OILMAST = 0x20,
    OILSHARP = 0x21,
    OILDEATH = 0x22,
    OILSKILL = 0x23,
    OILBSMTH = 0x24,
    OILFORT = 0x25,
    OILPERM = 0x26,
    OILHARD = 0x27,
    OILIMP = 0x28,
    OILLAST = 0x29,
    MAPOFDOOM = 0x2A,
    EAR = 0x2B,
    SPECELIX = 0x2C,
    INVALID = -1,
  };

  enum struct ItemEffectID {
    TOHIT = 0x0,
    TOHIT_CURSE = 0x1,
    DAMP = 0x2,
    DAMP_CURSE = 0x3,
    TOHIT_DAMP = 0x4,
    TOHIT_DAMP_CURSE = 0x5,
    ACP = 0x6,
    ACP_CURSE = 0x7,
    FIRERES = 0x8,
    LIGHTRES = 0x9,
    MAGICRES = 0xA,
    ALLRES = 0xB,
    SPLCOST = 0xC,
    SPLDUR = 0xD,
    SPLLVLADD = 0xE,
    CHARGES = 0xF,
    FIREDAM = 0x10,
    LIGHTDAM = 0x11,
    STR = 0x13,
    STR_CURSE = 0x14,
    MAG = 0x15,
    MAG_CURSE = 0x16,
    DEX = 0x17,
    DEX_CURSE = 0x18,
    VIT = 0x19,
    VIT_CURSE = 0x1A,
    ATTRIBS = 0x1B,
    ATTRIBS_CURSE = 0x1C,
    GETHIT_CURSE = 0x1D,
    GETHIT = 0x1E,
    LIFE = 0x1F,
    LIFE_CURSE = 0x20,
    MANA = 0x21,
    MANA_CURSE = 0x22,
    DUR = 0x23,
    DUR_CURSE = 0x24,
    INDESTRUCTIBLE = 0x25,
    LIGHT = 0x26,
    LIGHT_CURSE = 0x27,
    MULT_ARROWS = 0x29,
    FIRE_ARROWS = 0x2A,
    LIGHT_ARROWS = 0x2B,
    INVCURS = 0x2C,
    THORNS = 0x2D,
    NOMANA = 0x2E,
    NOHEALPLR = 0x2F,
    FIREBALL = 0x32,
    ABSHALFTRAP = 0x34,
    KNOCKBACK = 0x35,
    NOHEALMON = 0x36,
    STEALMANA = 0x37,
    STEALLIFE = 0x38,
    TARGAC = 0x39,
    FASTATTACK = 0x3A,
    FASTRECOVER = 0x3B,
    FASTBLOCK = 0x3C,
    DAMMOD = 0x3D,
    RNDARROWVEL = 0x3E,
    SETDAM = 0x3F,
    SETDUR = 0x40,
    NOMINSTR = 0x41,
    SPELL = 0x42,
    FASTSWING = 0x43,
    ONEHAND = 0x44,
    THREEXDAMVDEM = 0x45,
    ALLRESZERO = 0x46,
    DRAINLIFE = 0x48,
    RNDSTEALLIFE = 0x49,
    INFRAVISION = 0x4A,
    SETAC = 0x4B,
    ADDACLIFE = 0x4C,
    ADDMANAAC = 0x4D,
    FIRERESCLVL = 0x4E,
    AC_CURSE = 0x4F,
    INVALID = -1,
  };

  enum struct ItemSpecialEffect {
    NONE = 0x00000000,
    INFRAVISION = 0x00000001,
    RNDSTEALLIFE = 0x00000002,
    RNDARROWVEL = 0x00000004,
    FIRE_ARROWS = 0x00000008,
    FIREDAM = 0x00000010,
    LIGHTDAM = 0x00000020,
    DRAINLIFE = 0x00000040,
    UNKNOWN_1 = 0x00000080,
    NOHEALPLR = 0x00000100,
    UNKNOWN_2 = 0x00000200,
    UNKNOWN_3 = 0x00000400,
    KNOCKBACK = 0x00000800,
    NOHEALMON = 0x00001000,
    STEALMANA_3 = 0x00002000,
    STEALMANA_5 = 0x00004000,
    STEALLIFE_3 = 0x00008000,
    STEALLIFE_5 = 0x00010000,
    QUICKATTACK = 0x00020000,
    FASTATTACK = 0x00040000,
    FASTERATTACK = 0x00080000,
    FASTESTATTACK = 0x00100000,
    FASTRECOVER = 0x00200000,
    FASTERRECOVER = 0x00400000,
    FASTESTRECOVER = 0x00800000,
    FASTBLOCK = 0x01000000,
    LIGHT_ARROWS = 0x02000000,
    THORNS = 0x04000000,
    NOMANA = 0x08000000,
    ABSHALFTRAP = 0x10000000,
    UNKNOWN_4 = 0x20000000,
    THREE3XDAMVDEM = 0x40000000,
    ALLRESZERO = 0x80000000,
  };

  enum struct ItemCursorGraphic {
    POTION_OF_FULL_MANA = 0,
    SCROLL_OF = 1,
    GOLD_SMALL = 4,
    GOLD_MEDIUM = 5,
    GOLD_LARGE = 6,
    RING_OF_TRUTH = 10,
    RING = 12,
    SPECTRAL_ELIXIR = 15,
    GOLDEN_ELIXIR = 17,
    EMPYREAN_BAND = 18,
    EAR_SORCEROR = 19,
    EAR_WARRIOR = 20,
    EAR_ROGUE = 21,
    BLOOD_STONE = 25,
    ELIXIR_OF_VITALITY = 31,
    POTION_OF_HEALING = 32,
    POTION_OF_FULL_REJUVENATION = 33,
    ELIXIR_OF_MAGIC = 34,
    POTION_OF_FULL_HEALING = 35,
    ELIXIR_OF_DEXTERITY = 36,
    POTION_OF_REJUVENATION = 37,
    ELIXIR_OF_STRENGTH = 38,
    POTION_OF_MANA = 39,
    BRAIN = 40,
    OPTIC_AMULET = 44,
    AMULET = 45,
    DAGGER = 51,
    BLADE = 56,
    BASTARD_SWORD = 57,
    MACE = 59,
    LONG_SWORD = 60,
    BROAD_SWORD = 61,
    FALCHION = 62,
    MORNING_STAR = 63,
    SHORT_SWORD = 64,
    CLAYMORE = 65,
    CLUB = 66,
    SABRE = 67,
    SPIKED_CLUB = 70,
    SCIMITAR = 72,
    FULL_HELM = 75,
    MAGIC_ROCK = 76,
    THE_UNDEAD_CROWN = 78,
    HELM = 82,
    BUCKLER = 83,
    VIEL_OF_STEEL = 85,
    BOOK_GREY = 86,
    BOOK_RED = 87,
    BOOK_BLUE = 88,
    BLACK_MUSHROOM = 89,
    SKULL_CAP = 90,
    CAP = 91,
    HARLEQUIN_CREST = 93,
    CROWN = 95,
    MAP_OF_THE_STARS = 96,
    FUNGAL_TOME = 97,
    GREAT_HELM = 98,
    BATTLE_AXE = 101,
    HUNTERS_BOW = 102,
    FIELD_PLATE = 103,
    SMALL_SHIELD = 105,
    CLEAVER = 106,
    STUDDED_LEATHER_ARMOR = 107,
    SHORT_STAFF = 109,
    TWO_HANDED_SWORD = 110,
    CHAIN_MAIL = 111,
    SMALL_AXE = 112,
    KITE_SHIELD = 113,
    SCALE_MAIL = 114,
    SHORT_BOW = 118,
    LONG_WAR_BOW = 119,
    WAR_HAMMER = 121,
    MAUL = 122,
    LONG_STAFF = 123,
    WAR_STAFF = 124,
    TAVERN_SIGN = 126,
    HARD_LEATHER_ARMOR = 127,
    RAGS = 128,
    QUILTED_ARMOR = 129,
    FLAIL = 131,
    TOWER_SHIELD = 132,
    COMPOSITE_BOW = 133,
    GREAT_SWORD = 134,
    LEATHER_ARMOR = 135,
    SPLINT_MAIL = 136,
    ROBE = 137,
    ANVIL_OF_FURY = 140,
    BROAD_AXE = 141,
    LARGE_AXE = 142,
    GREAT_AXE = 143,
    AXE = 144,
    LARGE_SHIELD = 147,
    GOTHIC_SHIELD = 148,
    CLOAK = 149,
    CAPE = 150,
    FULL_PLATE_MAIL = 151,
    GOTHIC_PLATE = 152,
    BREAST_PLATE = 153,
    RING_MAIL = 154,
    STAFF_OF_LAZARUS = 155,
    ARKAINES_VALOR = 157,
    SHORT_WAR_BOW = 165,
    COMPOSITE_STAFF = 166,
    SHORT_BATTLE_BOW = 167,
    GOLD = 168,
  };

  struct ItemData
  {
    int ID;
    int _itype;
    int _ix;
    int _iy;

    bool _iIdentified;
    char _iMagical;
    std::string _iName;
    std::string _iIName;
    char _iClass;
    int _iCurs;
    int _ivalue;
    int _iMinDam;
    int _iMaxDam;
    int _iAC;
    int _iFlags;
    int _iMiscId;
    int _iSpell;

    int _iCharges;
    int _iMaxCharges;

    int _iDurability;
    int _iMaxDur;

    int _iPLDam;
    int _iPLToHit;
    int _iPLAC;
    int _iPLStr;
    int _iPLMag;
    int _iPLDex;
    int _iPLVit;
    int _iPLFR;
    int _iPLLR;
    int _iPLMR;
    int _iPLMana;
    int _iPLHP;
    int _iPLDamMod;
    int _iPLGetHit;
    int _iPLLight;
    char _iSplLvlAdd;

    int _iFMinDam;
    int _iFMaxDam;
    int _iLMinDam;
    int _iLMaxDam;

    char _iPrePower;
    char _iSufPower;

    char _iMinStr;
    char _iMinMag;
    char _iMinDex;
    bool _iStatFlag;
    int IDidx;
  };
}
