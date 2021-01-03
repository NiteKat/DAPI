#include "Command.h"

namespace DAPI
{
  Command Command::walkXY(int _targetX, int _targetY)
  {
    Command c{ CommandType::WALKXY };
    c.targetX = _targetX;
    c.targetY = _targetY;
    return c;
  }

  Command Command::talk(std::shared_ptr<Towner> target)
  {
    if (!target)
      return Command{ CommandType::NUM_CMDS };

    Command c{ CommandType::TALKXY };
    c.targetX = target->getX();
    c.targetY = target->getY();
    return c;
  }

  Command Command::selectStoreOption(StoreOption option)
  {
    Command c{ CommandType::SELECTSTOREOPTION };
    c.param1 = static_cast<int>(option);
    return c;
  }

  Command Command::buyItem(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::BUYITEM };
    c.param1 = item->getID();
    return c;
  }

  Command Command::sellItem(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::SELLITEM };
    c.param1 = item->getID();
    return c;
  }

  Command Command::rechargeItem(Item item)
  {
    Command c{ CommandType::RECHARGEITEM };
    c.param1 = item.getID();
    return c;
  }

  Command Command::repairItem(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::REPAIRITEM };
    c.param1 = item->getID();
    return c;
  }

  Command Command::attackMonster(Monster monster)
  {
    Command c{ CommandType::ATTACKID };
    c.param1 = monster.data->index;
    return c;
  }

  Command Command::attackXY(int targetX, int targetY)
  {
    Command c{ CommandType::ATTACKXY };
    c.targetX = targetX;
    c.targetY = targetY;
    return c;
  }

  Command Command::operateObject(Object object)
  {
    Command c{ CommandType::OPERATEOBJ };
    /*switch (object.getType())
    {
    case ObjectType::L1LDOOR:
    case ObjectType::L1RDOOR:
    case ObjectType::L2LDOOR:
    case ObjectType::L2RDOOR:
    case ObjectType::L3LDOOR:
    case ObjectType::L3RDOOR:
      if (object.getDoorState() == DoorState::CLOSED)
        c.type = CommandType::OPENDOOR;
      else
        c.type = CommandType::CLOSEDOOR;
      break;
    default:
      c.type = CommandType::OPERATEOBJ;
    }*/

    c.param1 = object.data->index;
    return c;
  }

  Command Command::toggleCharacterScreen()
  {
    Command c{ CommandType::CHARACTERSCREEN };
    return c;
  }

  Command Command::useBeltItem(int slot)
  {
    Command c{ CommandType::NUM_CMDS };
    c.param1 = slot;
    return c;
  }

  Command Command::getItem(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::GETITEM };
    c.param1 = item->getID();
    return c;
  }

  Command Command::setSpell(SpellID id, SpellType type)
  {
    Command c{ CommandType::SETSPELL };
    c.param1 = static_cast<int>(id);
    c.param2 = static_cast<int>(type.getID());
    return c;
  }

  Command Command::castMonster(Monster monster)
  {
    Command c{ CommandType::SPELLID };
    c.param1 = monster.data->index;
    return c;
  }

  Command Command::castXY(int _targetX, int _targetY)
  {
    Command c{ CommandType::SPELLXY };
    c.targetX = _targetX;
    c.targetY = _targetY;
    return c;
  }

  Command Command::toggleInventoryScreen()
  {
    Command c{ CommandType::INVENTORYSCREEN };
    return c;
  }

  Command Command::putInCursor(int ID)
  {
    Command c{ CommandType::PUTINCURSOR };
    c.param1 = ID;
    return c;
  }

  Command Command::putCursorItem(int location)
  {
    Command c{ CommandType::PUTCURSORITEM };
    c.param1 = location;
    return c;
  }

  Command Command::dropCursorItem()
  {
    Command c{ CommandType::DROPITEM };
    return c;
  }

  Command Command::useItem(int ID)
  {
    Command c{ CommandType::USEITEM };
    c.param1 = ID;
    return c;
  }

  Command Command::identifyStoreItem(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::IDENTIFYSTOREITEM };
    c.param1 = item->getID();
    return c;
  }

  Command Command::castSpellXY(int x, int y)
  {
    Command c{ CommandType::SPELLXY };
    c.param1 = x;
    c.param2 = y;
    return c;
  }

  Command Command::cancelQText()
  {
    Command c{ CommandType::CANCELQTEXT };
    return c;
  }

  Command Command::setFPS(int fps)
  {
    Command c{ CommandType::SETFPS };
    c.param1 = fps;
    return c;
  }

  Command Command::disarmTrap(Object object)
  {
    Command c{ CommandType::DISARMXY };
    c.param1 = object.data->index;
    return c;
  }

  Command Command::skillRepair(std::shared_ptr<Item> item)
  {
    Command c{ CommandType::SKILLREPAIR };
    c.param1 = item->data->ID;
    return c;
  }

  Command Command::increaseStat(int stat)
  {
    Command c;
    switch (stat)
    {
    case 0:
      c.type = CommandType::ADDSTR;
      break;
    case 1:
      c.type = CommandType::ADDMAG;
      break;
    case 2:
      c.type = CommandType::ADDDEX;
      break;
    case 3:
      c.type = CommandType::ADDVIT;
      break;
    }

    return c;
  }
}