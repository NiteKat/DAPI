#pragma once
#include <memory>
#include "ObjectData.h"

namespace DAPI
{
  struct Object
  {
    Object() { data = std::make_shared<ObjectData>(); }
    int getX() { return data->x; }
    int getY() { return data->y; }
    ObjectType getType() { return data->type; }
    ShrineType getShrineType() { return data->shrineType; }
    bool isSolid() { return data->solid; }
    DoorState getDoorState() { return data->doorState; }
    bool isSelectable() { return data->selectable; }

    std::shared_ptr<ObjectData> data;
  };
}