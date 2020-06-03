#pragma once

#include <map>
#include <vector>

#include "Player.h"
#include "Item.h"
#include "Towner.h"
#include "Trigger.h"

namespace DAPI
{
  enum struct StoreOption
  {
    TALK,
    IDENTIFYANITEM,
    EXIT,
    HEAL,
    BUYITEMS,
    WIRTPEEK,
    BUYBASIC,
    BUYPREMIUM,
    SELL,
    REPAIR,
    RECHARGE,
    BACK
  };

  enum struct TalkID {
    NONE = 0x0,
    SMITH = 0x1,
    SBUY = 0x2,
    SSELL = 0x3,
    SREPAIR = 0x4,
    WITCH = 0x5,
    WBUY = 0x6,
    WSELL = 0x7,
    WRECHARGE = 0x8,
    NOMONEY = 0x9,
    NOROOM = 0xA,
    CONFIRM = 0xB,
    BOY = 0xC,
    BBOY = 0xD,
    HEALER = 0xE,
    STORY = 0xF,
    HBUY = 0x10,
    SIDENTIFY = 0x11,
    SPBUY = 0x12,
    GOSSIP = 0x13,
    IDSHOW = 0x14,
    TAVERN = 0x15,
    DRUNK = 0x16,
    BARMAID = 0x17,
  };

  struct GameData
  {
    int player;
    char stextflag;
    int pauseMode;
    bool menuOpen;
    int pcurs;
    bool chrflag;
    bool invflag;
    bool qtextflag;

    std::map<int, PlayerData> playerList;
    std::vector<ItemData> itemList;
    std::vector<int> groundItems;
    std::vector<TownerData> townerList;
    std::vector<StoreOption> storeList;
    std::vector<int> storeItems;
    std::vector<TriggerData> triggerList;
  };
}