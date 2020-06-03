#pragma once
#include<memory>

#include "TriggerData.h"

namespace DAPI
{
  struct Trigger
  {
    Trigger() { data = std::make_shared<TriggerData>(); }

    int getLevel() { return data->lvl; }
    int getX() { return data->x; }
    int getY() { return data->y; }
    TriggerType getType() { return data->type; }

    std::shared_ptr<TriggerData> data;
  };
}