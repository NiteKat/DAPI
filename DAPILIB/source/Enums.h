namespace DAPI
{

  enum struct EquipSlot
  {
    HEAD = 0,
    RIGHTRING = 1,
    LEFTRING = 2,
    AMULET = 3,
    LEFTHAND = 4,
    RIGHTHAND = 5,
    BODY = 6,
    INV1 = 7,
    INV2 = 8,
    INV3 = 9,
    INV4 = 10,
    INV5 = 11,
    INV6 = 12,
    INV7 = 13,
    INV8 = 14,
    INV9 = 15,
    INV10 = 16,
    INV11 = 17,
    INV12 = 18,
    INV13 = 19,
    INV14 = 20,
    INV15 = 21,
    INV16 = 22,
    INV17 = 23,
    INV18 = 24,
    INV19 = 25,
    INV20 = 26,
    INV21 = 27,
    INV22 = 28,
    INV23 = 29,
    INV24 = 30,
    INV25 = 31,
    INV26 = 32,
    INV27 = 33,
    INV28 = 34,
    INV29 = 35,
    INV30 = 36,
    INV31 = 37,
    INV32 = 38,
    INV33 = 39,
    INV34 = 40,
    INV35 = 41,
    INV36 = 42,
    INV37 = 43,
    INV38 = 44,
    INV39 = 45,
    INV40 = 46,
    BELT1 = 47,
    BELT2 = 48,
    BELT3 = 49,
    BELT4 = 50,
    BELT5 = 51,
    BELT6 = 52,
    BELT7 = 53,
    BELT8 = 54
  };

  enum struct CursorType
  {
    NONE = 0x0,
    HAND = 0x1,
    IDENTIFY = 0x2,
    REPAIR = 0x3,
    RECHARGE = 0x4,
    DISARM = 0x5,
    OIL = 0x6,
    TELEKINESIS = 0x7,
    RESURRECT = 0x8,
    TELEPORT = 0x9,
    HEALOTHER = 0xA,
    HOURGLASS = 0xB,
    FIRSTITEM = 0xC,
  };

  enum struct CommandType
  {
    STAND,
    WALKXY,
    ACK_PLRINFO,
    ADDSTR,
    ADDMAG,
    ADDDEX,
    ADDVIT,
    SBSPELL,
    GETITEM,
    AGETITEM,
    PUTITEM,
    RESPAWNITEM,
    ATTACKXY,
    RATTACKXY,
    SPELLXY,
    TSPELLXY,
    OPOBJXY,
    DISARMXY,
    ATTACKID,
    ATTACKPID,
    RATTACKID,
    RATTACKPID,
    SPELLID,
    SPELLPID,
    TSPELLID,
    TSPELLPID,
    RESURRECT,
    OPOBJT,
    KNOCKBACK,
    TALKXY,
    NEWLVL,
    WARP,
    CHEAT_EXPERIENCE,
    CHEAT_SPELL_LEVEL,
    DEBUG,
    SYNCDATA,
    MONSTDEATH,
    MONSTDAMAGE,
    PLRDEAD,
    REQUESTGITEM,
    REQUESTAGITEM,
    GOTOGETITEM,
    GOTOAGETITEM,
    OPENDOOR,
    CLOSEDOOR,
    OPERATEOBJ,
    PLROPOBJ,
    BREAKOBJ,
    CHANGEPLRITEMS,
    DELPLRITEMS,
    PLRDAMAGE,
    PLRLEVEL,
    DROPITEM,
    PLAYER_JOINLEVEL,
    SEND_PLRINFO,
    SATTACKXY,
    ACTIVATEPORTAL,
    DEACTIVATEPORTAL,
    DLEVEL_0,
    DLEVEL_1,
    DLEVEL_2,
    DLEVEL_3,
    DLEVEL_4,
    DLEVEL_5,
    DLEVEL_6,
    DLEVEL_7,
    DLEVEL_8,
    DLEVEL_9,
    DLEVEL_10,
    DLEVEL_11,
    DLEVEL_12,
    DLEVEL_13,
    DLEVEL_14,
    DLEVEL_15,
    DLEVEL_16,
    DLEVEL_JUNK,
    DLEVEL_END,
    HEALOTHER,
    STRING,
    SETSTR,
    SETMAG,
    SETDEX,
    SETVIT,
    RETOWN,
    SPELLXYD,
    ITEMEXTRA,
    SYNCPUTITEM,
    KILLGOLEM,
    SYNCQUEST,
    ENDSHIELD,
    AWAKEGOLEM,
    NOVA,
    SETSHIELD,
    REMSHIELD,
    FAKE_SETID,
    FAKE_DROPID,
    SELECTSTOREOPTION,
    BUYITEM,
    SELLITEM,
    RECHARGEITEM,
    REPAIRITEM,
    BUYBASIC,
    CHARACTERSCREEN,
    SETSPELL,
    INVENTORYSCREEN,
    PUTINCURSOR,
    PUTCURSORITEM,
    USEITEM,
    IDENTIFYSTOREITEM,
    CANCELQTEXT,
    SETFPS,
    SKILLREPAIR,
    SKILLRECHARGE,
    TOGGLEMENU,
    NUM_CMDS
  };

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
}