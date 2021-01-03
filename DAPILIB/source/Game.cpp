#include "Game.h"

#include "Towner.h"

namespace DAPI
{
  Game::Game(ProtoClient& client) : client(client)
  {
    data = std::make_shared<GameData>();
    for (int x = -8; x < 9; x++)
    {
      switch (x)
      {
      case 8:
        panelScreenCheck[std::make_pair(x, 3)] = true;
        break;
      case 7:
        for (int y = 2; y < 5; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 6:
        for (int y = 1; y < 6; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 5:
        for (int y = 0; y < 7; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 4:
        for (int y = -1; y < 8; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 3:
        for (int y = -2; y < 9; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 2:
        for (int y = -3; y < 8; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 1:
        for (int y = -4; y < 7; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case 0:
        for (int y = -5; y < 6; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -1:
        for (int y = -6; y < 5; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -2:
        for (int y = -7; y < 4; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -3:
        for (int y = -8; y < 3; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -4:
        for (int y = -7; y < 2; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -5:
        for (int y = -6; y < 1; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -6:
        for (int y = -5; y < 0; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -7:
        for (int y = -4; y < -1; y++)
          panelScreenCheck[std::make_pair(x, y)] = true;
        break;
      case -8:
        panelScreenCheck[std::make_pair(x, -3)] = true;
        break;
      }
    }
  }

  std::vector<std::shared_ptr<Item>> Game::getItemsOnGround()
  {
    std::vector<std::shared_ptr<Item>> returnValue;
    for (auto& itemID : data->groundItems)
    {
      returnValue.push_back(std::make_shared<Item>(data->itemList[itemID]));
    }
    return returnValue;
  }

  Quest Game::getQuest(QuestID id)
  {
    for (int i = 0; i < data->questList.size(); i++)
    {
      if (data->questList[i].getID() == id)
        return data->questList[i];
    }
  }

  std::vector<std::shared_ptr<Towner>> Game::getTowners()
  {
    std::vector<std::shared_ptr<Towner>> returnValue;
    for (auto& townie : data->townerData)
    {
      if (townie.second.getTalkerType() != TalkerType::NONE)
        returnValue.push_back(std::make_shared<Towner>(townie.second));
    }
    return returnValue;
  }

  std::vector<StoreOption> Game::getStoreOptions()
  {
    return data->storeList;
  }

  std::shared_ptr<Tile> Game::getDungeonPiece(int x, int y)
  {
    if (x >= 112 || y >= 112 || x < 0 || y < 0)
      return nullptr;

    return std::make_shared<Tile>(data->dPiece[x][y]);
  }

  std::vector<std::shared_ptr<Item>> Game::getStoreItems()
  {
    std::vector<std::shared_ptr<Item>> returnValue;
    int i = 0;
    for (auto& itemID : data->storeItems)
    {
      returnValue.push_back(std::make_shared<Item>(data->itemList[itemID]));
      if (static_cast<TalkID>(data->stextflag) == TalkID::SIDENTIFY)
        returnValue[i]->data->_ivalue = 100;
    }
    return returnValue;
  }

  std::vector<Trigger> Game::getTriggers()
  {
    std::vector<Trigger> returnValue;
    for (auto& trigger : data->triggerList)
    {
      returnValue.push_back(trigger);
    }
    return returnValue;
  }

  std::vector<Monster> Game::getMonsters()
  {
    std::vector<Monster> returnValue;
    for (auto& monster : data->monsterList)
    {
      returnValue.push_back(monster);
    }
    return returnValue;
  }

  std::vector<Object> Game::getObjects()
  {
    std::vector<Object> returnValue;
    for (auto& object : data->objectList)
      returnValue.push_back(object);
    return returnValue;
  }

  std::vector<Missile> Game::getMissiles()
  {
    return data->missileList;
  }

  std::vector<Portal> Game::getPortals()
  {
    return data->portalList;
  }

  std::vector<Quest> Game::getQuests()
  {
    return data->questList;
  }

  bool Game::OKToAct()
  {
    return !data->stextflag && data->pauseMode != 2 && !data->menuOpen && static_cast<CursorType>(data->cursor) == CursorType::HAND && !data->qtextflag;
  }

  bool Game::OKToActStore()
  {
    return data->pauseMode != 2 && !data->menuOpen;
  }

  bool Game::characterScreen()
  {
    return data->chrflag;
  }

  bool Game::inventoryScreen()
  {
    return data->invflag;
  }

  bool Game::OKToPlaceItem()
  {
    return !data->stextflag && data->pauseMode != 2 && !data->menuOpen && static_cast<CursorType>(data->cursor) >= CursorType::FIRSTITEM;
  }

  bool Game::isOnScreen(int x, int y)
  {
    bool returnValue = false;
    int dx = data->playerList[data->player].getX() - x;
    int dy = data->playerList[data->player].getY() - y;
    if (!data->chrflag && !data->invflag)
    {
      if (dy > 0)
      {
        if (dx < 1 && abs(dx) + abs(dy) < 11)
          returnValue = true;
        else if (dx > 0 && abs(dx) + abs(dy) < 12)
          returnValue = true;
      }
      else
      {
        if ((dx > -1 || dy == 0) && abs(dx) + abs(dy) < 11)
          returnValue = true;
        else if ((dx < 0 && dy != 0) && abs(dx) + abs(dy) < 12)
          returnValue = true;
      }
    }
    else if ((data->chrflag && !data->invflag) || (!data->chrflag && data->invflag))
    {
      returnValue = panelScreenCheck[std::make_pair(dx, dy)];
    }
    return returnValue;
  }

  bool Game::walkXY(int targetX, int targetY)
  {
    if (!isOnScreen(targetX, targetY))
      return false;

    if (!OKToAct())
      return false;

    return this->issueCommand(Command::walkXY(targetX, targetY));
  }

  bool Game::talk(std::shared_ptr<Towner> target)
  {
    if (!target)
      return false;

    if (!isOnScreen(target->getX(), target->getY()))
      return false;

    if (!OKToAct())
      return false;

    return this->issueCommand(Command::talk(target));
  }

  bool Game::selectStoreOption(StoreOption option)
  {
    if (!data->stextflag)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& o : data->storeList)
    {
      if (o == option)
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    return this->issueCommand(Command::selectStoreOption(option));
  }

  bool Game::buyItem(std::shared_ptr<Item> item)
  {
    if (static_cast<TalkID>(data->stextflag) != TalkID::WBUY && static_cast<TalkID>(data->stextflag) != TalkID::SBUY && static_cast<TalkID>(data->stextflag) != TalkID::SPBUY && static_cast<TalkID>(data->stextflag) != TalkID::HBUY)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& storeItem : data->storeItems)
    {
      if (storeItem == item->getID())
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    return this->issueCommand(Command::buyItem(item));
  }

  bool Game::sellItem(std::shared_ptr<Item> item)
  {
    if (static_cast<TalkID>(data->stextflag) != TalkID::WSELL && static_cast<TalkID>(data->stextflag) != TalkID::SSELL)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& storeItem : data->storeItems)
    {
      if (storeItem == item->getID())
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    return this->issueCommand(Command::sellItem(item));
  }

  bool Game::rechargeItem(Item item)
  {
    if (static_cast<TalkID>(data->stextflag) != TalkID::WRECHARGE)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& storeItem : data->storeItems)
    {
      if (storeItem == item.getID())
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    //if (data->playerList[data->player].getGold() < item.getValue())
    //  return false;

    return this->issueCommand(Command::rechargeItem(item));
  }

  bool Game::repairItem(std::shared_ptr<Item> item)
  {
    if (static_cast<TalkID>(data->stextflag) != TalkID::SREPAIR)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& storeItem : data->storeItems)
    {
      if (storeItem == item->getID())
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    return this->issueCommand(Command::repairItem(item));
  }

  bool Game::attackMonster(Monster monster)
  {
    if (!isOnScreen(monster.getX(), monster.getY()))
      return false;

    if (!OKToAct())
      return false;

    return this->issueCommand(Command::attackMonster(monster));
  }

  bool Game::attackXY(int targetX, int targetY)
  {
    if (!isOnScreen(targetX, targetY))
      return false;

    if (!OKToAct())
      return false;

    return this->issueCommand(Command::attackXY(targetX, targetY));
  }

  bool Game::operateObject(Object object)
  {
    if (!isOnScreen(object.getX(), object.getY()))
      return false;

    if (!OKToAct())
      return false;

    bool found = false;
    for (auto ob : data->objectList)
    {
      if (ob.data->index == object.data->index)
      {
        found = true;
        break;
      }
    }

    if (!found)
      return false;

    return this->issueCommand(Command::operateObject(object));
  }

  bool Game::useBeltItem(int slot)
  {
    if (!OKToAct())
      return false;

    if (data->playerList[data->player].data->SpdList[slot] == nullptr)
      return false;

    return this->issueCommand(Command::useBeltItem(slot));
  }

  bool Game::toggleCharacterScreen()
  {
    if (!OKToAct())
      return false;

    return this->issueCommand(Command::toggleCharacterScreen());
  }

  bool Game::increaseStrength()
  {
    if (!OKToAct())
      return false;

    if (!data->chrflag)
      return false;

    if (data->playerList[data->player].getStatPoints() == 0)
      return false;

    int maxValue;

    switch (data->playerList[data->player].getClass().getID())
    {
    case ClassID::WARRIOR:
      maxValue = 250;
      break;
    case ClassID::ROGUE:
      maxValue = 55;
      break;
    case ClassID::SORCERER:
      maxValue = 45;
      break;
    }

    if (data->playerList[data->player].getBaseStrength() < maxValue)
      return this->issueCommand(Command::increaseStat(0));

    return false;
  }

  bool Game::increaseMagic()
  {
    if (!OKToAct())
      return false;

    if (!data->chrflag)
      return false;

    if (data->playerList[data->player].getStatPoints() == 0)
      return false;

    int maxValue;

    switch (data->playerList[data->player].getClass().getID())
    {
    case ClassID::WARRIOR:
      maxValue = 50;
      break;
    case ClassID::ROGUE:
      maxValue = 70;
      break;
    case ClassID::SORCERER:
      maxValue = 250;
      break;
    }

    if (data->playerList[data->player].getBaseMagic() < maxValue)
      return this->issueCommand(Command::increaseStat(1));

    return false;
  }

  bool Game::increaseDexterity()
  {
    if (!OKToAct())
      return false;

    if (!data->chrflag)
      return false;

    if (data->playerList[data->player].getStatPoints() == 0)
      return false;

    int maxValue;

    switch (data->playerList[data->player].getClass().getID())
    {
    case ClassID::WARRIOR:
      maxValue = 60;
      break;
    case ClassID::ROGUE:
      maxValue = 250;
      break;
    case ClassID::SORCERER:
      maxValue = 85;
      break;
    }

    if (data->playerList[data->player].getBaseDexterity() < maxValue)
      return this->issueCommand(Command::increaseStat(2));

    return false;
  }

  bool Game::increaseVitality()
  {
    if (!OKToAct())
      return false;

    if (!data->chrflag)
      return false;

    if (data->playerList[data->player].getStatPoints() == 0)
      return false;

    int maxValue;

    switch (data->playerList[data->player].getClass().getID())
    {
    case ClassID::WARRIOR:
      maxValue = 100;
      break;
    case ClassID::ROGUE:
      maxValue = 80;
      break;
    case ClassID::SORCERER:
      maxValue = 80;
      break;
    }

    if (data->playerList[data->player].getBaseVitality() < maxValue)
      return this->issueCommand(Command::increaseStat(3));

    return false;
  }

  bool Game::pickupItemFromGround(std::shared_ptr<Item> item)
  {
    if (!OKToAct())
      return false;

    bool found = false;

    for (auto i = 0; i < data->groundItems.size(); i++)
    {
      if (data->groundItems[i] == item->getID())
        found = true;
      if (found)
        break;
    }

    if (!found)
      return false;

    if (!isOnScreen(item->getX(), item->getY()))
      return false;

    return this->issueCommand(Command::getItem(item));
  }

  bool Game::setSpell(Spell spell)
  {
    switch (spell.getType().getID())
    {
    case SpellTypeID::SKILL:
      if (!(data->playerList[data->player].data->_pAblSpells & (1 << (static_cast<int>(spell.getID()) - 1))))
        return false;
      break;
    case SpellTypeID::SPELL:
      if (!(data->playerList[data->player].data->_pMemSpells & (1 << (static_cast<int>(spell.getID()) - 1))))
        return false;
      break;
    case SpellTypeID::SCROLL:
      if (!(data->playerList[data->player].data->_pScrlSpells & (1 << (static_cast<int>(spell.getID()) - 1))))
        return false;
      break;
    case SpellTypeID::CHARGES:
      if (data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_LEFT) &&
        (data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_LEFT)->getSpell() != spell.getID() ||
          data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_LEFT)->getCharges() == 0))
        return false;

      if (data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_RIGHT) &&
        (data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_RIGHT)->getSpell() != spell.getID() ||
          data->playerList[data->player].getEquippedItem(InvBodyLocation::HAND_RIGHT)->getCharges() == 0))
        return false;
      break;
    case SpellTypeID::INVALID:
    default:
      return false;
      break;
    }

    return this->issueCommand(Command::setSpell(spell.getID(), spell.getType()));
  }

  bool Game::castSpell(Monster monster)
  {
    if (!isOnScreen(monster.getX(), monster.getY()))
      return false;

    if (!OKToAct())
      return false;

    return this->issueCommand(Command::castMonster(monster));
  }

  bool Game::toggleInventoryScreen()
  {
    if (!OKToAct())
      return false;

    return this->issueCommand(Command::toggleInventoryScreen());
  }

  bool Game::putInCursor(std::shared_ptr<Item> item)
  {
    if (!item)
      return false;

    if (!data->invflag)
      return false;

    if (!OKToAct())
      return false;

    for (int i = 0; i < 7; i++)
    {
      auto ei = data->playerList[data->player].getEquippedItem(static_cast<DAPI::InvBodyLocation>(i));
      if (ei && item->getID() == ei->getID())
        return this->issueCommand(Command::putInCursor(item->getID()));
    }
    for (auto& i : data->playerList[data->player].getInventoryItems())
    {
      if (item->getID() == i->getID())
        return this->issueCommand(Command::putInCursor(item->getID()));
    }
    for (auto& i : data->playerList[data->player].getSpeedItems())
    {
      if (i && item->getID() == i->getID())
        return this->issueCommand(Command::putInCursor(item->getID()));
    }

    return false;
  }

  bool Game::putCursorItem(EquipSlot location)
  {
    if (!data->invflag)
      return false;

    if (!OKToPlaceItem())
      return false;

    return this->issueCommand(Command::putCursorItem(static_cast<int>(location)));
  }

  bool Game::dropCursorItem()
  {
    if (!isOnScreen(data->playerList[data->player].getX(), data->playerList[data->player].getY()))
      return false;

    return this->issueCommand(Command::dropCursorItem());
  }

  bool Game::useItem(std::shared_ptr<Item> item)
  {
    if (!OKToAct())
      return false;

    auto beltList = self().getSpeedItems();
    for (int i = 0; i < beltList.size(); i++)
    {
      if (item == beltList[i])
        return useBeltItem(i);
    }

    if (!data->invflag)
      return false;

    auto invList = self().getInventoryItems();
    for (int i = 0; i < invList.size(); i++)
    {
      if (item == invList[i])
        return this->issueCommand(Command::useItem(item->getID()));
    }

    return false;
  }

  bool Game::identifyStoreItem(std::shared_ptr<Item> item)
  {
    if (static_cast<TalkID>(data->stextflag) != TalkID::SIDENTIFY)
      return false;

    if (!OKToActStore())
      return false;

    bool found = false;
    for (auto& storeItem : data->storeItems)
    {
      if (storeItem == item->getID())
      {
        found = true;
        break;
      }
    }
    if (!found)
      return false;

    return this->issueCommand(Command::identifyStoreItem(item));
  }
  
  bool Game::castSpellXY(int x, int y)
  {
    if (!isOnScreen(x, y))
      return false;

    if (!OKToAct())
      return false;

    if (x < 0 || 111 < x || y < 0 || 111 < y)
      return false;

    return this->issueCommand(Command::castSpellXY(x, y));
  }

  bool Game::cancelQText()
  {
    if (!data->qtextflag)
      return false;

    return this->issueCommand(Command::cancelQText());
  }

  bool Game::setFPS(int fps)
  {
    return this->issueCommand(Command::setFPS(fps));
  }

  bool Game::disarmTrap(Object object)
  {
    if (static_cast<DAPI::CursorType>(data->cursor) != DAPI::CursorType::DISARM)
      return false;

    if (data->playerList[data->player].getClass().getID() != DAPI::ClassID::ROGUE)
      return false;

    return this->issueCommand(Command::disarmTrap(object));

  }

  bool Game::skillRepair(std::shared_ptr<Item> item)
  {
    if (static_cast<DAPI::CursorType>(data->cursor) != DAPI::CursorType::REPAIR)
      return false;

    if (data->playerList[data->player].getClass().getID() != DAPI::ClassID::WARRIOR)
      return false;

    if (!data->invflag)
      return false;

    for (auto &bodyItem : data->playerList[data->player].data->InvBody)
    {
      if (!bodyItem.second)
        continue;

      if (bodyItem.second->getID() == item->getID())
        return client.issueCommand(Command::skillRepair(item));
    }
    for (auto& invItem : data->playerList[data->player].data->InvList)
    {
      if (invItem->getID() == item->getID())
        return client.issueCommand(Command::skillRepair(item));
    }
    return false;
  }

  bool Game::issueCommand(Command command)
  {
    return client.issueCommand(command);
  }
}