#pragma once
#include "DiabloStructs.h"

namespace DAPI
{
  struct TriggerData
  {
    bool compare(DiabloInternal::TriggerStruct& other) { return (other._tlvl == lvl && other._tmsg == type && other._tx == x && other._ty == y); }

    int ID;
    int x;
    int y;
    int lvl;
    int type;
  };
}