#pragma once
#include <string>

namespace DAPI
{
  enum struct TalkerType {
    SMITH = 0x0,
    HEALER = 0x1,
    DEADGUY = 0x2,
    TAVERN = 0x3,
    STORY = 0x4,
    DRUNK = 0x5,
    WITCH = 0x6,
    BMAID = 0x7,
    PEGBOY = 0x8,
    COW = 0x9,
    PRIEST = 0xA,
    NONE = 11
  };

  struct TownerData
  {
    int ID;
    TalkerType _ttype;
    int _tx;
    int _ty;
    std::string _tName;
  };
}