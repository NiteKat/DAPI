#pragma once

namespace DAPI
{
  enum struct QuestID {
    ROCK = 0x0,
    MUSHROOM = 0x1,
    GARBUD = 0x2,
    ZHAR = 0x3,
    VEIL = 0x4,
    DIABLO = 0x5,
    BUTCHER = 0x6,
    LTBANNER = 0x7,
    BLIND = 0x8,
    BLOOD = 0x9,
    ANVIL = 0xA,
    WARLORD = 0xB,
    SKELKING = 0xC,
    PWATER = 0xD,
    SCHAMB = 0xE,
    BETRAYER = 0xF
  };

  typedef enum QuestState {
    QUEST_NOTAVAIL = 0,
    QUEST_ACTIVE = 2,
  } quest_state;

  struct QuestData
  {
    QuestID id;
    QuestState state;
  };
}