#pragma once
#include "PortalData.h"

namespace DAPI
{
  struct Portal
  {
    Portal();
    int getX() { return data->x; }
    int getY() { return data->y; }
    std::shared_ptr<Player> getPlayer() { return data->player; }
    std::shared_ptr<PortalData> data;
  };
}