#pragma once
#include <memory>

#include "GameData.h"
#include "Command.h"
#include "../../DAPIFrontendClient/ProtoClient.h"

namespace DAPI
{

  struct Game
  {
    friend struct ProtoClient;
    Game(ProtoClient& client);

    const Backend connectedTo() { return data->backend; }
    Player& self() { return data->playerList[data->player]; }
    TalkID getTalkID() { return static_cast<TalkID>(data->stextflag); }
    std::shared_ptr<Tile> getDungeonPiece(int x, int y);
    bool questTextFlag() { return data->qtextflag; }
    std::string getQuestText() { return data->qtext; }
    int getDungeonLevel() { return data->currlevel; }
    bool isSetLevel() { return data->setlevel; }
    Quest getQuest(QuestID id);
    int getFPS() { return data->FPS; }
    int getCursor() { return data->cursor; }
    GameMode getGameMode() { return data->gameMode; }
    Difficulty getDifficulty() { return data->gnDifficulty; }

    std::vector<std::shared_ptr<Item>> getItemsOnGround();
    std::vector<std::shared_ptr<Towner>> getTowners();
    std::vector<StoreOption> getStoreOptions();
    std::vector<std::shared_ptr<Item>> getStoreItems();
    std::vector<Trigger> getTriggers();
    std::vector<Monster> getMonsters();
    std::vector<std::shared_ptr<Player>> getPlayers();
    std::vector<Object> getObjects();
    std::vector<Missile> getMissiles();
    std::vector<Portal> getPortals();
    std::vector<Quest> getQuests();

    bool isOnScreen(int x, int y);
    bool OKToAct();
    bool OKToActStore();
    bool characterScreen();
    bool inventoryScreen();
    bool OKToPlaceItem();

    bool walkXY(int targetX, int targetY);
    bool talk(std::shared_ptr<Towner> target);
    bool selectStoreOption(StoreOption option);
    bool buyItem(std::shared_ptr<Item> item);
    bool sellItem(std::shared_ptr<Item> item);
    bool rechargeItem(Item item);
    bool repairItem(std::shared_ptr<Item> item);
    bool attackMonster(Monster monster);
    bool attackXY(int targetX, int targetY);
    bool operateObject(Object object);
    bool useBeltItem(int slot);
    bool toggleCharacterScreen();
    bool identifyItem(std::shared_ptr<Item> item);
    bool increaseStrength();
    bool increaseMagic();
    bool increaseDexterity();
    bool increaseVitality();
    bool pickupItemFromGround(std::shared_ptr<Item> item);
    bool setSpell(Spell spell);
    bool castSpell(Monster monster);
    bool toggleInventoryScreen();
    bool putInCursor(std::shared_ptr<Item> item);
    bool putCursorItem(EquipSlot location);
    bool dropCursorItem();
    bool useItem(std::shared_ptr<Item> item);
    bool identifyStoreItem(std::shared_ptr<Item> item);
    bool castSpellXY(int x, int y);
    bool cancelQText();
    bool setFPS(int fps);
    bool disarmTrap(Object object);
    bool skillRepair(std::shared_ptr<Item> item);
    bool skillRecharge(std::shared_ptr<Item> item);
    bool isMenuOpen();
    bool toggleMenu();
    bool saveGame();
    bool quit();
    bool clearCursor();
    bool sendChat(std::string message);
    bool issueCommand(Command command);

    
    std::map<std::pair<int, int>, bool> panelScreenCheck;

    ProtoClient& client;
    std::shared_ptr<GameData> data;
  };
}