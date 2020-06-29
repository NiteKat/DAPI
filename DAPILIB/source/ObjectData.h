#pragma once

namespace DAPI
{
  enum struct ObjectType {
    L1LIGHT = 0x0,
    L1LDOOR = 0x1,
    L1RDOOR = 0x2,
    SKFIRE = 0x3,
    LEVER = 0x4,
    CHEST1 = 0x5,
    CHEST2 = 0x6,
    CHEST3 = 0x7,
    CANDLE1 = 0x8,
    CANDLE2 = 0x9,
    CANDLEO = 0xA,
    BANNERL = 0xB,
    BANNERM = 0xC,
    BANNERR = 0xD,
    SKPILE = 0xE,
    SKSTICK1 = 0xF,
    SKSTICK2 = 0x10,
    SKSTICK3 = 0x11,
    SKSTICK4 = 0x12,
    SKSTICK5 = 0x13,
    CRUX1 = 0x14,
    CRUX2 = 0x15,
    CRUX3 = 0x16,
    STAND = 0x17,
    ANGEL = 0x18,
    BOOK2L = 0x19,
    BCROSS = 0x1A,
    NUDEW2R = 0x1B,
    SWITCHSKL = 0x1C,
    TNUDEM1 = 0x1D,
    TNUDEM2 = 0x1E,
    TNUDEM3 = 0x1F,
    TNUDEM4 = 0x20,
    TNUDEW1 = 0x21,
    TNUDEW2 = 0x22,
    TNUDEW3 = 0x23,
    TORTURE1 = 0x24,
    TORTURE2 = 0x25,
    TORTURE3 = 0x26,
    TORTURE4 = 0x27,
    TORTURE5 = 0x28,
    BOOK2R = 0x29,
    L2LDOOR = 0x2A,
    L2RDOOR = 0x2B,
    TORCHL = 0x2C,
    TORCHR = 0x2D,
    TORCHL2 = 0x2E,
    TORCHR2 = 0x2F,
    SARC = 0x30,
    FLAMEHOLE = 0x31,
    FLAMELVR = 0x32,
    WATER = 0x33,
    BOOKLVR = 0x34,
    TRAPL = 0x35,
    TRAPR = 0x36,
    BOOKSHELF = 0x37,
    WEAPRACK = 0x38,
    BARREL = 0x39,
    BARRELEX = 0x3A,
    SHRINEL = 0x3B,
    SHRINER = 0x3C,
    SKELBOOK = 0x3D,
    BOOKCASEL = 0x3E,
    BOOKCASER = 0x3F,
    BOOKSTAND = 0x40,
    BOOKCANDLE = 0x41,
    BLOODFTN = 0x42,
    DECAP = 0x43,
    TCHEST1 = 0x44,
    TCHEST2 = 0x45,
    TCHEST3 = 0x46,
    BLINDBOOK = 0x47,
    BLOODBOOK = 0x48,
    PEDISTAL = 0x49,
    L3LDOOR = 0x4A,
    L3RDOOR = 0x4B,
    PURIFYINGFTN = 0x4C,
    ARMORSTAND = 0x4D,
    ARMORSTANDN = 0x4E,
    GOATSHRINE = 0x4F,
    CAULDRON = 0x50,
    MURKYFTN = 0x51,
    TEARFTN = 0x52,
    ALTBOY = 0x53,
    MCIRCLE1 = 0x54,
    MCIRCLE2 = 0x55,
    STORYBOOK = 0x56,
    STORYCANDLE = 0x57,
    STEELTOME = 0x58,
    WARARMOR = 0x59,
    WARWEAP = 0x5A,
    TBCROSS = 0x5B,
    WEAPONRACK = 0x5C,
    WEAPONRACKN = 0x5D,
    MUSHPATCH = 0x5E,
    LAZSTAND = 0x5F,
    SLAINHERO = 0x60,
    SIGNCHEST = 0x61,
    NULL_98 = 0x62,
  };

  enum struct ShrineType {
    MYSTERIOUS = 0x0,
    HIDDEN = 0x1,
    GLOOMY = 0x2,
    WEIRD = 0x3,
    MAGICAL = 0x4,
    STONE = 0x5,
    RELIGIOUS = 0x6,
    ENCHANTED = 0x7,
    THAUMATURGIC = 0x8,
    FASCINATING = 0x9,
    CRYPTIC = 0xA,
    MAGICAL2 = 0xB,
    ELDRITCH = 0xC,
    EERIE = 0xD,
    DIVINE = 0xE,
    HOLY = 0xF,
    SACRED = 0x10,
    SPIRITUAL = 0x11,
    SPOOKY = 0x12,
    ABANDONED = 0x13,
    CREEPY = 0x14,
    QUIET = 0x15,
    SECLUDED = 0x16,
    ORANTE = 0x17,
    GLIMMERING = 0x18,
    TAINTED = 0x19,
    NONE = -0x1
  };

  enum struct DoorState {
    CLOSED = 0x0,
    OPEN = 0x1,
    BLOCKED = 0x2,
    INVALID = -0x1
  };

  struct ObjectData
  {
    int x;
    int y;
    ObjectType type;
    ShrineType shrineType;
    bool solid;
    DoorState doorState;
    bool selectable;
    int index;
  };
}