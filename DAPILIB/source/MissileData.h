#pragma once

namespace DAPI
{

  enum struct MissileType
  {
    MIS_ARROW = 0x0,
    MIS_FIREBOLT = 0x1,
    MIS_GUARDIAN = 0x2,
    MIS_RNDTELEPORT = 0x3,
    MIS_LIGHTBALL = 0x4,
    MIS_FIREWALL = 0x5,
    MIS_FIREBALL = 0x6,
    MIS_LIGHTCTRL = 0x7,
    MIS_LIGHTNING = 0x8,
    MIS_MISEXP = 0x9,
    MIS_TOWN = 0xA,
    MIS_FLASH = 0xB,
    MIS_FLASH2 = 0xC,
    MIS_MANASHIELD = 0xD,
    MIS_FIREMOVE = 0xE,
    MIS_CHAIN = 0xF,
    MIS_SENTINAL = 0x10,
    MIS_BLODSTAR = 0x11,
    MIS_BONE = 0x12,
    MIS_METLHIT = 0x13,
    MIS_RHINO = 0x14,
    MIS_MAGMABALL = 0x15,
    MIS_LIGHTCTRL2 = 0x16,
    MIS_LIGHTNING2 = 0x17,
    MIS_FLARE = 0x18,
    MIS_MISEXP2 = 0x19,
    MIS_TELEPORT = 0x1A,
    MIS_FARROW = 0x1B,
    MIS_DOOMSERP = 0x1C,
    MIS_FIREWALLA = 0x1D,
    MIS_STONE = 0x1E,
    MIS_NULL_1F = 0x1F,
    MIS_INVISIBL = 0x20,
    MIS_GOLEM = 0x21,
    MIS_ETHEREALIZE = 0x22,
    MIS_BLODBUR = 0x23,
    MIS_BOOM = 0x24,
    MIS_HEAL = 0x25,
    MIS_FIREWALLC = 0x26,
    MIS_INFRA = 0x27,
    MIS_IDENTIFY = 0x28,
    MIS_WAVE = 0x29,
    MIS_NOVA = 0x2A,
    MIS_BLODBOIL = 0x2B,
    MIS_APOCA = 0x2C,
    MIS_REPAIR = 0x2D,
    MIS_RECHARGE = 0x2E,
    MIS_DISARM = 0x2F,
    MIS_FLAME = 0x30,
    MIS_FLAMEC = 0x31,
    MIS_FIREMAN = 0x32,
    MIS_KRULL = 0x33,
    MIS_CBOLT = 0x34,
    MIS_HBOLT = 0x35,
    MIS_RESURRECT = 0x36,
    MIS_TELEKINESIS = 0x37,
    MIS_LARROW = 0x38,
    MIS_ACID = 0x39,
    MIS_MISEXP3 = 0x3A,
    MIS_ACIDPUD = 0x3B,
    MIS_HEALOTHER = 0x3C,
    MIS_ELEMENT = 0x3D,
    MIS_RESURRECTBEAM = 0x3E,
    MIS_BONESPIRIT = 0x3F,
    MIS_WEAPEXP = 0x40,
    MIS_RPORTAL = 0x41,
    MIS_BOOM2 = 0x42,
    MIS_DIABAPOCA = 0x43,
  };

  struct MissileData
  {
    MissileType type;
    int mixvel;
    int miyvel;
    int mix;
    int miy;
    int misx;
    int misy;
  };
}