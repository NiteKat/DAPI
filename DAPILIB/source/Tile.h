#pragma once
#include<memory>

#include "TileData.h"

namespace DAPI
{
  struct Tile
  {
    Tile();

    int getType() { return data->type; }
    int getX() { return data->x; }
    int getY() { return data->y; }
    bool getSolid() { return data->solid || data->type == 0; }
    bool allowsMissile() { return !data->stopMissile; }

    std::shared_ptr<TileData> data;
  };
}