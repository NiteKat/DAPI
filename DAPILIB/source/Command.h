#pragma once

#include <memory>
#include "Towner.h"
#include "Enums.h"
#include "Item.h"
#include "Monster.h"
#include "Object.h"
#include "Spell.h"

namespace DAPI
{
  struct Command
  {
    Command() = default;
    Command(CommandType type) : type(type) {}

    static Command walkXY(int _targetX, int _targetY);
    static Command talk(std::shared_ptr<Towner> target);
    static Command selectStoreOption(StoreOption option);
    static Command buyItem(std::shared_ptr<Item> item);
    static Command sellItem(std::shared_ptr<Item> item);
    static Command rechargeItem(Item item);
    static Command repairItem(std::shared_ptr<Item> item);
    static Command attackMonster(Monster monster);
    static Command attackXY(int targetX, int targetY);
    static Command operateObject(Object object);
    static Command toggleCharacterScreen();
    static Command increaseStat(int stat);
    static Command useBeltItem(int slot);
    static Command getItem(std::shared_ptr<Item> item);
    static Command setSpell(SpellID id, SpellType type);
    static Command castMonster(Monster monster);
    static Command castXY(int _targetX, int _targetY);
    static Command toggleInventoryScreen();
    static Command putInCursor(int ID);
    static Command putCursorItem(int location);
    static Command dropCursorItem();
    static Command useItem(int ID);
    static Command identifyStoreItem(std::shared_ptr<Item> item);
    static Command castSpellXY(int x, int y);
    static Command cancelQText();
    static Command setFPS(int fps);
    static Command disarmTrap(Object object);
    static Command skillRepair(std::shared_ptr<Item> item);
    static Command skillRecharge(std::shared_ptr<Item> item);
    static Command toggleMenu();
    static Command saveGame();
    static Command quit();

    CommandType type;
    int targetX;
    int targetY;
    int param1;
    int param2;
  };
}