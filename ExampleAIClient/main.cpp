#include "..\DAPIFrontendClient\ProtoClient.h"
#include <chrono>
#include <thread>
#include <sstream>

#include "..\DAPILIB\source\DAPI.h"

std::chrono::system_clock::time_point lastDraw;
std::stringstream output;
bool dMonster[112][112];
bool dItem[112][112];
bool dMissile[112][112];
std::pair<bool, int> dObject[112][112];

std::pair<int, int> getItemSize(DAPI::ItemCursorGraphic cursor)
{
  auto curs = static_cast<int>(cursor);
  if (curs < 51 || curs == 168)
    return std::make_pair(1, 1);
  else if (curs == 51)
    return std::make_pair(1, 2);
  else if (curs < 75)
    return std::make_pair(1, 3);
  else if (curs < 101)
    return std::make_pair(2, 2);

  return std::make_pair(2, 3);
}

void clearScreen()
{
  //output << std::string(50, '\n');
  system("cls");
  memset(dMonster, 0, sizeof(dMonster));
  memset(dItem, 0, sizeof(dItem));
  memset(dObject, 0, sizeof(dObject));
  memset(dMissile, 0, sizeof(dMissile));
}

void displayInvGrid(DAPI::Game& Diablo)
{
  output << "inventory grid:" << std::endl;
  auto grid = Diablo.self().getInventoryGrid();
  for (int i = 0; i < 40; i++)
  {
    output << static_cast<int>(grid[i]) << " ";
    if (i == 9 || i == 19 || i == 29)
      output << std::endl;
  }
  output << std::endl;
  if (Diablo.getItemsOnGround().size())
  {
    auto size = getItemSize(Diablo.getItemsOnGround()[0]->getCursorGraphic());
    bool open[6] = { false };
    for (int y = 0; y < 4; y++)
    {
      if (y + (size.second - 1) > 3)
      {
        output << "Cannot fit" << std::endl;
        return;
      }
      for (int x = 0; x < 10; x++)
      {
        if (x + (size.first - 1) > 9)
          break;

        if (grid[x + 10 * y] == -1 && grid[x + 10 * (y + 1)] == -1 && grid[x + 10 * (y + 2)] == -1)
        {
          output << "Fits!" << std::endl;
          return;
        }
      }
    }
  }
}

void displayCharacter(DAPI::Game& Diablo)
{
  output << Diablo.self().getName() << " the " << Diablo.self().getClass().getName() << "'s stats:" << std::endl;
  output << "ID: " << Diablo.self().getID() << std::endl;
  output << "Level: " << Diablo.self().getLevel() << "\tExperience: " << Diablo.self().getExperience() << std::endl;
  output << "\t\tNext Level: " << Diablo.self().getExperienceToNextLevel() << std::endl;
  output << "\t\t\t    Gold" << std::endl;
  output << "Strength: " << Diablo.self().getBaseStrength() << " " << Diablo.self().getStrength() << "\t\t    " << Diablo.self().getGold() << std::endl;
  output << "Magic:    " << Diablo.self().getBaseMagic() << " " << Diablo.self().getMagic() << "\tArmor Class: " << Diablo.self().getArmorClass() << std::endl;
  output << "Dexterity:" << Diablo.self().getBaseDexterity() << " " << Diablo.self().getDexterity() << "\t To Hit: " << Diablo.self().getToHit() << std::endl;
  output << "Vitality: " << Diablo.self().getBaseVitality() << " " << Diablo.self().getVitality() << "\tDamage: " << Diablo.self().getMinimumDamage() << "-" << Diablo.self().getMaximumDamage() << std::endl;
  output << "Stat Pts: " << Diablo.self().getStatPoints() << "\tResist Magic: " << Diablo.self().getMagicResist() << std::endl;
  output << "Life:     " << Diablo.self().getMaxLife() << " " << Diablo.self().getLife() << "\tResist Fire: " << Diablo.self().getFireResist() << std::endl;
  output << "Mana:     " << Diablo.self().getMaxMana() << " " << Diablo.self().getMana() << "\tResist Lgtning:" << Diablo.self().getLightningResist() << std::endl;
  output << std::endl;
  output << "Spell List:" << std::endl;
  for (auto spell : Diablo.self().getAvailableSpells())
  {
    output << spell.getName() << "\t" << spell.getType().getName() << "\tMana Cost: " << spell.getManaCost() << "\tDamage: " << spell.getMinimumDamage() << " - " << spell.getMaximumDamage() << std::endl;
  }
}

void displayItemInfo(DAPI::Game& Diablo, std::shared_ptr<DAPI::Item> item)
{
  if (!item)
    return;

  switch (item->getType())
  {
  case DAPI::ItemType::NONE:
    break;
  case DAPI::ItemType::MISC:
    output << item->getName();
    break;
  case DAPI::ItemType::SWORD:
  case DAPI::ItemType::AXE:
  case DAPI::ItemType::BOW:
  case DAPI::ItemType::MACE:
    output << item->getName() << "\tDamage: " << item->getMinimumDamage() << "-" << item->getMaximumDamage() << "\tDur: " << item->getCurrentDurability() << "/" << item->getMaxDurability();
    break;
  case DAPI::ItemType::LARMOR:
  case DAPI::ItemType::SHIELD:
  case DAPI::ItemType::HELM:
    output << item->getName() << "\tArmor: " << item->getArmorClass() << "\tDur: " << item->getCurrentDurability() << "/" << item->getMaxDurability();
    break;
  case DAPI::ItemType::MARMOR:
    break;
  case DAPI::ItemType::HARMOR:
    break;
  case DAPI::ItemType::STAFF:
    output << item->getName() << "\tDamage: " << item->getMinimumDamage() << "-" << item->getMaximumDamage() << "\tDur: " << item->getCurrentDurability() << "/" << item->getMaxDurability();
    if (item->getSpell() != DAPI::SpellID::SPNULL)
      output << std::endl << "\t\t\tCharges: " << item->getCharges() << "/" << item->getMaxCharges();
    break;
  case DAPI::ItemType::GOLD:
    output << item->getValue() << " gold pieces";
    break;
  case DAPI::ItemType::RING:
    break;
  case DAPI::ItemType::AMULET:
    break;
  }
  if ((item->getRequiredStrength() && item->getRequiredStrength() != -1) || (item->getRequiredMagic() && item->getRequiredMagic() != -1))
    output << "\tRequired: ";
  if (item->getRequiredStrength() && item->getRequiredStrength() != -1)
    output << item->getRequiredStrength() << " STR ";
  if (item->getRequiredMagic() && item->getRequiredMagic() != -1)
    output << item->getRequiredMagic() << " MAG ";
  if (item->getRequiredDexterity() && item->getRequiredDexterity() != -1)
    output << item->getRequiredDexterity() << " DEX ";

  //output << "\tID: " << item->getID();
  //output << "\tx: " << Diablo.self().getX() - item->getX() << " y: " << Diablo.self().getY() - item->getY();
  if (item->getItemQuality() != DAPI::ItemQuality::NORMAL)
  {
    output << std::endl << "\t\t\t";
    if (item->getIsIdentified())
      output << item->getEffectText();
    else
      output << "not identified";
  }
}

void displayEquippedItems(DAPI::Game& Diablo)
{
  for (int i = 0; i < static_cast<int>(DAPI::InvBodyLocation::NUM_INVLOC); i++)
  {
    switch (static_cast<DAPI::InvBodyLocation>(i))
    {
    case DAPI::InvBodyLocation::HEAD:
      output << "Head:\t";
      break;
    case DAPI::InvBodyLocation::RING_LEFT:
      output << "Left Ring:";
      break;
    case DAPI::InvBodyLocation::RING_RIGHT:
      output << "Right Ring:";
      break;
    case DAPI::InvBodyLocation::AMULET:
      output << "Amulet:\t";
      break;
    case DAPI::InvBodyLocation::HAND_LEFT:
      output << "Left Hand:";
      break;
    case DAPI::InvBodyLocation::HAND_RIGHT:
      output << "Right Hand:";
      break;
    case DAPI::InvBodyLocation::CHEST:
      output << "Chest:\t";
      break;
    }
    output << "\t";
    auto item = Diablo.self().getEquippedItem(static_cast<DAPI::InvBodyLocation>(i));
    if (!item)
    {
      output << "Empty" << std::endl;
      continue;
    }
    displayItemInfo(Diablo, item);
    output << std::endl;
  }
  output << std::endl;
  output << "Current inventory:" << std::endl;
  for (auto& item : Diablo.self().getInventoryItems())
  {
    displayItemInfo(Diablo, item);
    output << std::endl;
  }
  output << std::endl;
  output << "Speed List: ";
  int i = 1;
  for (auto& item : Diablo.self().getSpeedItems())
  {
    output << "[" << i << "] ";
    displayItemInfo(Diablo, item);
    if (i != 8)
      output << " / ";
    i++;
  }
  output << std::endl;
  output << "Held Item:\t";
  if (Diablo.self().getCursorItem())
    displayItemInfo(Diablo, Diablo.self().getCursorItem());
  else
    output << "Empty";
  output << std::endl << "Items on ground: " << std::endl;
  for (auto& item : Diablo.getItemsOnGround())
  {
    displayItemInfo(Diablo, item);
    output << std::endl;
  }
}


void displayTowners(DAPI::Game& Diablo)
{
  output << std::endl;
  output << "Towners:" << std::endl;
  for (auto& townie : Diablo.getTowners())
  {
    output << townie->getName() << " x: " << townie->getX() << " y: " << townie->getY() << std::endl;
  }
}

void displayStoreOptions(DAPI::Game& Diablo)
{
  output << std::endl;
  output << "Store Options:" << std::endl;
  for (auto& option : Diablo.getStoreOptions())
  {
    switch (option)
    {
    case DAPI::StoreOption::BUYBASIC:
      output << "Buy Basic";
      break;
    case DAPI::StoreOption::BUYITEMS:
      output << "Buy Items";
      break;
    case DAPI::StoreOption::BUYPREMIUM:
      output << "Buy Premium";
      break;
    case DAPI::StoreOption::EXIT:
      output << "Exit";
      break;
    case DAPI::StoreOption::HEAL:
      output << "Heal";
      break;
    case DAPI::StoreOption::IDENTIFYANITEM:
      output << "Identify an Item";
      break;
    case DAPI::StoreOption::RECHARGE:
      output << "Recharge Staves";
      break;
    case DAPI::StoreOption::REPAIR:
      output << "Repair";
      break;
    case DAPI::StoreOption::SELL:
      output << "Sell Items";
      break;
    case DAPI::StoreOption::TALK:
      output << "Talk";
      break;
    case DAPI::StoreOption::WIRTPEEK:
      output << "What have ya got?";
      break;
    case DAPI::StoreOption::BACK:
      output << "Back";
      break;
    default:
      break;
    }
    output << std::endl;
  }
  output << "Store items:" << std::endl;
  for (auto& item : Diablo.getStoreItems())
  {
    displayItemInfo(Diablo, item);
    output << "\tValue: " << item->getValue();
    output << std::endl;
  }
}

void reconnect(DAPI::ProtoClient& protoClient)
{
  while (!protoClient.connect())
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
}

bool buy;
bool heal;

void testStore(DAPI::Game& Diablo)
{
  if (Diablo.getTalkID() != DAPI::TalkID::NONE)
  {
    for (auto& option : Diablo.getStoreOptions())
    {
      if (!buy && option == DAPI::StoreOption::BUYITEMS)
      {
        Diablo.selectStoreOption(option);
        break;
      }
      else if (!heal && option == DAPI::StoreOption::HEAL)
      {
        heal = true;
        Diablo.selectStoreOption(option);
        break;
      }
      else if (Diablo.getTalkID() == DAPI::TalkID::HBUY && buy && option == DAPI::StoreOption::BACK)
      {
        Diablo.selectStoreOption(option);
        break;
      }
    }
    if (Diablo.getTalkID() == DAPI::TalkID::HBUY && !buy)
    {
      Diablo.buyItem(*Diablo.getStoreItems().begin());
      buy = true;
    }
  }
}

void displaySolidMap(DAPI::Game& Diablo)
{
  for (auto& m : Diablo.getMonsters())
    dMonster[m.getX()][m.getY()] = true;
  for (auto& i : Diablo.getItemsOnGround())
    dItem[i->getX()][i->getY()] = true;
  auto objectList = Diablo.getObjects();
  for (int i = 0; i < objectList.size(); i++)
  {
    dObject[objectList[i].getX()][objectList[i].getY()].first = true;
    dObject[objectList[i].getX()][objectList[i].getY()].second = i;
  }
  auto missileList = Diablo.getMissiles();
  for (auto& mi : Diablo.getMissiles())
    dMissile[mi.getX()][mi.getY()] = true;
  for (int y = Diablo.self().getY() - 13; y < 112 && y < Diablo.self().getY() + 14; y++)
  {
    for (int x = Diablo.self().getX() - 13; x < 112 && x < Diablo.self().getX() + 14; x++)
    {
      if (x == Diablo.self().getX() && y == Diablo.self().getY())
        output << "P ";
      else
      {
        auto tile = Diablo.getDungeonPiece(x, y);
        if (tile)
        {

          if (dMonster[x][y])
            output << "M ";
          else if (dMissile[x][y])
            output << ". ";
          else if (dItem[x][y])
            output << "I ";
          else if (tile->getSolid())
            output << "X ";
          else if (dObject[x][y].first)
          {
            switch (objectList[dObject[x][y].second].getType())
            {
            case DAPI::ObjectType::CHEST1:
            case DAPI::ObjectType::CHEST2:
            case DAPI::ObjectType::CHEST3:
              output << "C ";
              break;
            case DAPI::ObjectType::BARREL:
              output << "B ";
              break;
            default:
              output << "O ";
              break;
            }
          }
          else if (tile->getType() == -1)
            output << "? ";
          else
            output << "  ";
        }
      }
    }
    output << std::endl;
  }
}

void displayTriggers(DAPI::Game& Diablo)
{
  output << "Trigger List:" << std::endl;
  for (auto& trigger : Diablo.getTriggers())
  {
    output << "Trigger at: (" << trigger.getX() << ", " << trigger.getY() << ") type: ";
    switch (trigger.getType())
    {
    case DAPI::TriggerType::DIABNEXTLVL:
      output << "Next Level";
      break;
    case DAPI::TriggerType::DIABPREVLVL:
      output << "Previous Level";
      break;
    case DAPI::TriggerType::DIABTOWNWARP:
      output << "Town Warp to Level " << trigger.getLevel();
      break;
    case DAPI::TriggerType::DIABTWARPUP:
      output << "Back to town " << trigger.getLevel();
      break;
    default:
      output << "I dunno?";
      break;
    }
    output << std::endl;
  }
}

void displayMonsters(DAPI::Game& Diablo)
{
  output << std::endl <<  "Monster List:" << std::endl;
  for (auto& m : Diablo.getMonsters())
  {
    output << "Name: " << m.getName() << " X: " << m.getX() << " Y: " << m.getY() << std::endl;
  }
}

void increaseStats(DAPI::Game& Diablo)
{
  if (!Diablo.characterScreen())
    Diablo.toggleCharacterScreen();
  if (Diablo.self().getStatPoints() > 0)
    Diablo.increaseVitality();
}

int main(int argc, const char* argv[])
{
  int door = 5;
  buy = heal = false;
  lastDraw = std::chrono::system_clock::now();
  DAPI::ProtoClient protoClient;
  DAPI::Game Diablo(protoClient);
  reconnect(protoClient);
  bool didthething = false;
  while (true)
  {
    protoClient.update(Diablo);
    if (protoClient.isConnected())
    {
      if (std::chrono::system_clock::now() - lastDraw > std::chrono::milliseconds{ 333 })
      {
        clearScreen();
        output << "Still connected!" << std::endl;
        //displayCharacter(Diablo);
        //displayEquippedItems(Diablo);
        //displayTowners(Diablo);
        //Diablo.walkXY(Diablo.self().getX(), Diablo.self().getY() + 1);
        /*if (Diablo.OKToAct())
          output << "It's okay to act!";
        else
          output << "It's NOT OKAY TO ACT! PANIC! Not at the disco.";*/
        //displayStoreOptions(Diablo);
        //if (Diablo.getStoreOptions().size())
        //  Diablo.selectStoreOption(DAPI::StoreOption::RECHARGE);
        //testStore(Diablo);
        displaySolidMap(Diablo);
        //displayTriggers(Diablo);
        //displayMonsters(Diablo);
        //output << Diablo.self().getX() << ", " << Diablo.self().getY();
        //displayInvGrid(Diablo);
        std::cout << output.str();
        output.str(std::string());
        output.clear();
        lastDraw = std::chrono::system_clock::now();
      }
      //for (auto& item : Diablo.self().getInventoryItems())
      //{
      //  if (item->getMiscID() == DAPI::ItemMiscID::HEAL)
      //    Diablo.useItem(item);
      //}
      /*if (Diablo.getMonsters().size())
        Diablo.castSpell(Diablo.getMonsters()[0]);//Diablo.attackXY(Diablo.getMonsters().begin()->getFutureX(), Diablo.getMonsters().begin()->getFutureY());
      else
      {
        bool done = false;
        if (door < 5)
          door++;
        else
        {
          for (auto& o : Diablo.getObjects())
          {
            switch (o.getType())
            {
            case DAPI::ObjectType::L1LDOOR:
            case DAPI::ObjectType::L1RDOOR:
            case DAPI::ObjectType::L2LDOOR:
            case DAPI::ObjectType::L2RDOOR:
            case DAPI::ObjectType::L3LDOOR:
            case DAPI::ObjectType::L3RDOOR:
              if (o.getDoorState() == DAPI::DoorState::CLOSED)
              {
                Diablo.operateObject(o);
                done = true;
                door = 0;
              }
              break;
            default:
              if (o.isSelectable())
              {
                Diablo.operateObject(o);
                done = true;
              }
              break;
            }
            if (done)
              break;
          }
        }
      }*/
    }
    else
    {
      while (!protoClient.isConnected())
        reconnect(protoClient);
    }
  }
}