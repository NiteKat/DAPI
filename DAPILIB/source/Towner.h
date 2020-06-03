#pragma once
#include <memory>

#include "TownerData.h"

namespace DAPI
{
  struct Towner
  {
    Towner();
    int getX() { return data->_tx; }
    int getY() { return data->_ty; }
    TalkerType getTalkerType() { return data->_ttype; }
    std::string getName() { return data->_tName; }

    std::shared_ptr<TownerData> data;
  };
}