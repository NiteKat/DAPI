#include "Tile.h"

namespace DAPI
{
  Tile::Tile()
  {
    data = std::make_shared<TileData>();
    data->type = -1;
    data->solid = false;
    data->x = -1;
    data->y = -1;
  }
}
