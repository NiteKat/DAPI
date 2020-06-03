#pragma once
#include <memory>

#include "MonsterData.h"

namespace DAPI
{
  struct Monster
  {
    Monster() { data = std::make_shared<MonsterData>(); }

    int getX() { return data->x; }
    int getY() { return data->y; }
    int getFutureX() { return data->futx; }
    int getFutureY() { return data->futy; }
    std::string getName() { return data->name; }
    MonsterType getType() { return data->type; }
    int getKills() { return data->kills; }

    std::shared_ptr<MonsterData> data;
  };
}