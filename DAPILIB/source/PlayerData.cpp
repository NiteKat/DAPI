#include "PlayerData.h"

namespace DAPI
{
  std::string classNames[] = { "Warrior", "Rogue", "Sorcerer",
  "Number of Classes", "Unknown" };
  std::string CharacterClass::getName()
  {
    return classNames[static_cast<int>(ID)];
  }
}