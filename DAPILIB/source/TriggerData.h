#pragma once


namespace DAPI
{
  enum struct TriggerType {
    DIABNEXTLVL = 0x402,
    DIABPREVLVL = 0x403,
    DIABRTNLVL = 0x404,
    DIABSETLVL = 0x405,
    DIABWARPLVL = 0x406,
    DIABTOWNWARP = 0x407,
    DIABTWARPUP = 0x408,
    DIABRETOWN = 0x409,
    DIABNEWGAME = 0x40A,
    DIABLOADGAME = 0x40B
  };

  struct TriggerData
  {
    int lvl;
    int x;
    int y;
    TriggerType type;
  };
}