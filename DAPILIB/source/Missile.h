#pragma once
#include <memory>

#include "MissileData.h"

namespace DAPI
{
  struct Missile
  {
    Missile() { data = std::make_shared<MissileData>(); }

    MissileType getType() { return data->type; }
    int getXVelocity() { return data->mixvel; }
    int getYVelocity() { return data->miyvel; }
    int getX() { return data->mix; }
    int getY() { return data->miy; }
    int getSourceX() { return data->misx; }
    int getSourceY() { return data->misy; }

    std::shared_ptr<MissileData> data;
  };
}