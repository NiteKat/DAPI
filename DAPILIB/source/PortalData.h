#pragma once
#include<memory>

#include "Player.h"

namespace DAPI
{
  struct PortalData
  {
    PortalData() : x(0), y(0), player(nullptr) {}
    int x;
    int y;
    std::shared_ptr<Player> player;
  };
}
