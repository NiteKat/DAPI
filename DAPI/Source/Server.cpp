#include "Server.h"

#include "DiabloStructs.h"

namespace DAPI
{
  Server::Server() : FPS(20), gameClock(0)
  {
    output.open("output.csv");
    data = std::make_unique<GameData>();
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

  void Server::update()
  {
    gameClock += 1000 / FPS;
    if (isConnected())
    {
      //std::stringstream test;
      //test << "\\x" << std::hex << msDelay;
      //Patch((char*)0x440EBE, test.str().c_str(), 1);
      //Patch((char*)0x440F53, test.str().c_str(), 1);
      updateGameData();
      protoClient.transmitMessages();
      protoClient.receiveMessages();
      processMessages();
    }
    else
    {
      checkForConnections();
    }
  }

  bool Server::isConnected() const
  {
    return protoClient.isConnected();
  }

  void Server::processMessages()
  {
    auto stextsel = reinterpret_cast<int*>(0x6A8A28);
    auto STextEnter = reinterpret_cast<void(*)()>(0x45BF34);
    auto stext = reinterpret_cast<DiabloInternal::STextStruct(*)>(0x69FB40);

    bool issuedCommand = false;
    while (protoClient.messageQueueSize())
    {
      auto message = protoClient.getNextMessage();
      if (message.get() == nullptr)
        return;
      if (message->has_endofqueue())
        return;

      if (message->has_command() && !issuedCommand)
      {
        auto command = message->command();
        if (command.has_move() && this->OKToAct())
        {
          auto moveMessage = command.move();
          this->move(moveMessage.targetx(), moveMessage.targety());
        }
        else if (command.has_talk() && this->OKToAct())
        {
          auto talkMessage = command.talk();
          this->talk(talkMessage.targetx(), talkMessage.targety());
        }
        else if (command.has_option() && this->data->stextflag)
        {
          auto option = command.option();
          this->selectStoreOption(static_cast<StoreOption>(command.option().option()));
        }
        else if (command.has_buyitem())
        {
          auto buyItem = command.buyitem();
          this->buyItem(buyItem.id());
        }
        else if (command.has_sellitem())
        {
          auto sellItem = command.sellitem();
          this->sellItem(sellItem.id());
        }
        else if (command.has_rechargeitem())
        {
          auto rechargeItem = command.rechargeitem();
          this->rechargeItem(rechargeItem.id());
        }
        else if (command.has_repairitem())
        {
          auto repairItem = command.repairitem();
          this->repairItem(repairItem.id());
        }
        else if (command.has_attackmonster())
        {
          auto attackMonster = command.attackmonster();
          this->attackMonster(attackMonster.index());
        }
        else if (command.has_attackxy())
        {
          auto attackXY = command.attackxy();
          this->attackXY(attackXY.x(), attackXY.y());
        }
        else if (command.has_operateobject())
        {
          auto operateObject = command.operateobject();
          this->operateObject(operateObject.index());
        }
        else if (command.has_usebeltitem())
        {
          auto useBeltItem = command.usebeltitem();
          this->useBeltItem(useBeltItem.slot());
        }
        else if (command.has_togglecharactersheet())
        {
          auto toggleCharacterSheet = command.togglecharactersheet();
          this->toggleCharacterScreen();
        }
        else if (command.has_increasestat())
        {
          auto increaseStat = command.increasestat();
          this->increaseStat(static_cast<CommandType>(increaseStat.stat()));
        }
        else if (command.has_getitem())
        {
          auto getItem = command.getitem();
          this->getItem(getItem.id());
        }
        else if (command.has_setspell())
        {
          auto setSpell = command.setspell();
          this->setSpell(setSpell.spellid(), static_cast<DiabloInternal::SpellTypeID>(setSpell.spelltype()));
        }
        else if (command.has_castmonster())
        {
          auto castMonster = command.castmonster();
          this->castSpell(castMonster.index());
        }
        else if (command.has_toggleinventory())
        {
          this->toggleInventory();
        }
        else if (command.has_putincursor())
        {
          auto putInCursor = command.putincursor();
          this->putInCursor(putInCursor.id());
        }
        else if (command.has_putcursoritem())
        {
          auto putCursorItem = command.putcursoritem();
          this->putCursorItem(putCursorItem.target());
        }
        else if (command.has_dropcursoritem())
        {
          this->dropCursorItem();
        }
        else if (command.has_useitem())
        {
          auto useItem = command.useitem();
          this->useItem(useItem.id());
        }
        else if (command.has_identifystoreitem())
        {
          auto identifyStoreItem = command.identifystoreitem();
          this->identifyStoreItem(identifyStoreItem.id());
        }
        else if (command.has_castxy())
        {
          auto castXY = command.castxy();
          this->castSpell(castXY.x(), castXY.y());
        }
        else if (command.has_cancelqtext())
        {
          this->cancelQText();
        }
        else if (command.has_disarmtrap())
        {
          auto disarmTrap = command.disarmtrap();
          this->disarmTrap(disarmTrap.index());
        }
        else if (command.has_skillrepair())
        {
          auto skillRepair = command.skillrepair();
          this->skillRepair(skillRepair.id());
        }
        else if (command.has_skillrecharge())
        {
          auto skillRecharge = command.skillrecharge();
          this->skillRecharge(skillRecharge.id());
        }
        else if (command.has_togglemenu())
        {
          this->toggleMenu();
        }
        else if (command.has_savegame())
        {
          this->saveGame();
        }
        else if (command.has_quit())
        {
          this->quit();
        }
        else if (command.has_clearcursor())
        {
          this->clearCursor();
        }
        issuedCommand = true;
        if (command.has_setfps())
        {
          auto setfps = command.setfps();
          this->setFPS(setfps.fps());
          issuedCommand = false;
        }
      }
    }
  }

  void Server::checkForConnections()
  {
    if (isConnected())
      return;
    if (!listening)
    {
      protoClient.initListen();
      listening = false;
    }
    protoClient.checkForConnection();
    if (!protoClient.isConnected())
      return;
    protoClient.stopListen();
  }

  void Server::updateGameData()
  {
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
    auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
    auto item = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x635A28);
    auto itemactive = reinterpret_cast<int(*)>(0x634CC8);
    auto numitems = reinterpret_cast<int(*)>(0x6414DC);
    auto dItem = reinterpret_cast<char(*)[112][112]>(0x5C9A10);
    auto towner = reinterpret_cast<DiabloInternal::TownerStruct(*)>(0x6AAC38);
    auto numtowners = reinterpret_cast<int(*)>(0x6AAC20);
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto PauseMode = reinterpret_cast<int(*)>(0x525740);
    auto gmenu_is_on = reinterpret_cast<bool(*)()>(0x419FE8);
    auto stext = reinterpret_cast<DiabloInternal::STextStruct(*)>(0x69FB40);
    auto storehold = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A09F0);
    auto witchitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A4EF8);
    auto smithitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A8A40);
    auto premiumitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x69F290);
    auto healitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A6BC0);
    auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
    auto nSolidTable = reinterpret_cast<BOOLEAN(*)>(0x5BB2F0);
    auto ScrollInfo = reinterpret_cast<DiabloInternal::ScrollStruct(*)>(0x5BDAF8);
    auto trigs = reinterpret_cast<DiabloInternal::TriggerStruct(*)>(0x6ABAE0);
    auto nummonsters = reinterpret_cast<int(*)>(0x64D328);
    auto monstactive = reinterpret_cast<int(*)>(0x64D008);
    auto monster = reinterpret_cast<DiabloInternal::MonsterStruct(*)>(0x64D330);
    auto object = reinterpret_cast<DiabloInternal::ObjectStruct(*)>(0x679C38);
    auto chrflag = reinterpret_cast<BOOL(*)>(0x4B896C);
    auto nummissiles = reinterpret_cast<int(*)>(0x64C6F0);
    auto missileactive = reinterpret_cast<int(*)>(0x646D18);
    auto missile = reinterpret_cast<DiabloInternal::MissileStruct(*)>(0x647100);
    auto nMissileTable = reinterpret_cast<BOOLEAN(*)>(0x5CEB14);
    auto invflag = reinterpret_cast<BOOL(*)>(0x634CB8);
    auto portal = reinterpret_cast<DiabloInternal::PortalStruct(*)>(0x69BC98);
    auto qtextflag = reinterpret_cast<BOOLEAN(*)>(0x646D00);
    auto qtextptr = reinterpret_cast<char(**)>(0x646CF8);
    auto nobjects = reinterpret_cast<int(*)>(0x679A34);
    auto objectactive = reinterpret_cast<int(*)>(0x679838);
    auto currlevel = reinterpret_cast<BYTE(*)>(0x5BB1EE);
    auto setlevel = reinterpret_cast<BOOLEAN(*)>(0x5CF31D);
    auto setlvlnum = reinterpret_cast<BYTE(*)>(0x5CCB10);
    auto numtrigs = reinterpret_cast<int(*)>(0x6ABAD8);
    auto quests = reinterpret_cast<DiabloInternal::QuestStruct(*)>(0x69BD10);
    auto dObject = reinterpret_cast<char(*)[112][112]>(0x539C48);
    auto gbMaxPlayers = reinterpret_cast<BYTE(*)>(0x679660);

    auto fullFillItemInfo = [&](int itemID, DiabloInternal::ItemStruct* item) {

      data->itemList[itemID].ID = itemID;
      data->itemList[itemID]._iSeed = item->_iSeed;
      data->itemList[itemID]._iCreateInfo = item->_iCreateInfo;
      data->itemList[itemID]._itype = item->_itype;
      data->itemList[itemID]._ix = item->_ix;
      data->itemList[itemID]._iy = item->_iy;

      data->itemList[itemID]._iIdentified = item->_iIdentified;
      data->itemList[itemID]._iMagical = item->_iMagical;
      strcpy_s(data->itemList[itemID]._iName, item->_iName);
      if (data->itemList[itemID]._iIdentified)
      {
        strcpy_s(data->itemList[itemID]._iIName, item->_iIName);

        data->itemList[itemID]._iFlags = item->_iFlags;
        data->itemList[itemID]._iPrePower = item->_iPrePower;
        data->itemList[itemID]._iSufPower = item->_iSufPower;
        data->itemList[itemID]._iPLDam = item->_iPLDam;
        data->itemList[itemID]._iPLToHit = item->_iPLToHit;
        data->itemList[itemID]._iPLAC = item->_iPLAC;
        data->itemList[itemID]._iPLStr = item->_iPLStr;
        data->itemList[itemID]._iPLMag = item->_iPLMag;
        data->itemList[itemID]._iPLDex = item->_iPLDex;
        data->itemList[itemID]._iPLVit = item->_iPLVit;
        data->itemList[itemID]._iPLFR = item->_iPLFR;
        data->itemList[itemID]._iPLLR = item->_iPLLR;
        data->itemList[itemID]._iPLMR = item->_iPLMR;
        data->itemList[itemID]._iPLMana = item->_iPLMana;
        data->itemList[itemID]._iPLHP = item->_iPLHP;
        data->itemList[itemID]._iPLDamMod = item->_iPLDamMod;
        data->itemList[itemID]._iPLGetHit = item->_iPLGetHit;
        data->itemList[itemID]._iPLLight = item->_iPLLight;
        data->itemList[itemID]._iSplLvlAdd = item->_iSplLvlAdd;
        data->itemList[itemID]._iFMinDam = item->_iFMinDam;
        data->itemList[itemID]._iFMaxDam = item->_iFMaxDam;
        data->itemList[itemID]._iLMinDam = item->_iLMinDam;
        data->itemList[itemID]._iLMaxDam = item->_iLMaxDam;
      }
      else
      {
        strcpy_s(data->itemList[itemID]._iName, item->_iName);

        data->itemList[itemID]._iFlags = -1;
        data->itemList[itemID]._iPrePower = -1;
        data->itemList[itemID]._iSufPower = -1;
        data->itemList[itemID]._iPLDam = -1;
        data->itemList[itemID]._iPLToHit = -1;
        data->itemList[itemID]._iPLAC = -1;
        data->itemList[itemID]._iPLStr = -1;
        data->itemList[itemID]._iPLMag = -1;
        data->itemList[itemID]._iPLDex = -1;
        data->itemList[itemID]._iPLVit = -1;
        data->itemList[itemID]._iPLFR = -1;
        data->itemList[itemID]._iPLLR = -1;
        data->itemList[itemID]._iPLMR = -1;
        data->itemList[itemID]._iPLMana = -1;
        data->itemList[itemID]._iPLHP = -1;
        data->itemList[itemID]._iPLDamMod = -1;
        data->itemList[itemID]._iPLGetHit = -1;
        data->itemList[itemID]._iPLLight = -1;
        data->itemList[itemID]._iSplLvlAdd = -1;
        data->itemList[itemID]._iFMinDam = -1;
        data->itemList[itemID]._iFMaxDam = -1;
        data->itemList[itemID]._iLMinDam = -1;
        data->itemList[itemID]._iLMaxDam = -1;
      }
      data->itemList[itemID]._iClass = item->_iClass;
      data->itemList[itemID]._iCurs = item->_iCurs;
      if (item->_itype == 11)
        data->itemList[itemID]._ivalue = item->_ivalue;
      else
        data->itemList[itemID]._ivalue = -1;
      data->itemList[itemID]._iMinDam = item->_iMinDam;
      data->itemList[itemID]._iMaxDam = item->_iMaxDam;
      data->itemList[itemID]._iAC = item->_iAC;
      data->itemList[itemID]._iMiscId = item->_iMiscId;
      data->itemList[itemID]._iSpell = item->_iSpell;

      data->itemList[itemID]._iCharges = item->_iCharges;
      data->itemList[itemID]._iMaxCharges = item->_iMaxCharges;

      data->itemList[itemID]._iDurability = item->_iDurability;
      data->itemList[itemID]._iMaxDur = item->_iMaxDur;

      data->itemList[itemID]._iMinStr = item->_iMinStr;
      data->itemList[itemID]._iMinMag = item->_iMinMag;
      data->itemList[itemID]._iMinDex = item->_iMinDex;
      data->itemList[itemID]._iStatFlag = item->_iStatFlag;
      data->itemList[itemID].IDidx = item->IDidx;
    };

    auto partialFillItemInfo = [&](int itemID, DiabloInternal::ItemStruct* item) {

      data->itemList[itemID].ID = itemID;
      data->itemList[itemID]._iSeed = item->_iSeed;
      data->itemList[itemID]._iCreateInfo = item->_iCreateInfo;
      data->itemList[itemID]._itype = item->_itype;
      data->itemList[itemID]._ix = item->_ix;
      data->itemList[itemID]._iy = item->_iy;

      data->itemList[itemID]._iIdentified = item->_iIdentified;
      data->itemList[itemID]._iMagical = item->_iMagical;
      strcpy_s(data->itemList[itemID]._iName, item->_iName);
      if (data->itemList[itemID]._iIdentified)
        strcpy_s(data->itemList[itemID]._iIName, item->_iIName);
      else
        strcpy_s(data->itemList[itemID]._iName, item->_iName);
      data->itemList[itemID]._iFlags = -1;
      data->itemList[itemID]._iPrePower = -1;
      data->itemList[itemID]._iSufPower = -1;
      data->itemList[itemID]._iPLDam = -1;
      data->itemList[itemID]._iPLToHit = -1;
      data->itemList[itemID]._iPLAC = -1;
      data->itemList[itemID]._iPLStr = -1;
      data->itemList[itemID]._iPLMag = -1;
      data->itemList[itemID]._iPLDex = -1;
      data->itemList[itemID]._iPLVit = -1;
      data->itemList[itemID]._iPLFR = -1;
      data->itemList[itemID]._iPLLR = -1;
      data->itemList[itemID]._iPLMR = -1;
      data->itemList[itemID]._iPLMana = -1;
      data->itemList[itemID]._iPLHP = -1;
      data->itemList[itemID]._iPLDamMod = -1;
      data->itemList[itemID]._iPLGetHit = -1;
      data->itemList[itemID]._iPLLight = -1;
      data->itemList[itemID]._iSplLvlAdd = -1;
      data->itemList[itemID]._iFMinDam = -1;
      data->itemList[itemID]._iFMaxDam = -1;
      data->itemList[itemID]._iLMinDam = -1;
      data->itemList[itemID]._iLMaxDam = -1;
      data->itemList[itemID]._iClass = item->_iClass;
      data->itemList[itemID]._iCurs = item->_iCurs;
      if (item->_itype == 11)
        data->itemList[itemID]._ivalue = item->_ivalue;
      else
        data->itemList[itemID]._ivalue = -1;
      data->itemList[itemID]._iMinDam = -1;
      data->itemList[itemID]._iMaxDam = -1;
      data->itemList[itemID]._iAC = -1;
      data->itemList[itemID]._iMiscId = item->_iMiscId;
      data->itemList[itemID]._iSpell = item->_iSpell;

      data->itemList[itemID]._iCharges = -1;
      data->itemList[itemID]._iMaxCharges = -1;

      data->itemList[itemID]._iDurability = -1;
      data->itemList[itemID]._iMaxDur = -1;

      data->itemList[itemID]._iMinStr = -1;
      data->itemList[itemID]._iMinMag = -1;
      data->itemList[itemID]._iMinDex = -1;
      data->itemList[itemID]._iStatFlag = -1;
      data->itemList[itemID].IDidx = item->IDidx;
    };

    auto isOnScreen = [&](int x, int y) {
      int dx = plr[*myplr]._px - x;
      int dy = plr[*myplr]._py - y;
      if (!*chrflag && !*invflag)
      {
        if (dy > 0)
        {
          if (dx < 1 && abs(dx) + abs(dy) < 11)
          {
            return true;
          }
          else if (dx > 0 && abs(dx) + abs(dy) < 12)
          {
            return true;
          }
        }
        else
        {
          if ((dx > -1 || dy == 0) && abs(dx) + abs(dy) < 11)
          {
            return true;
          }
          else if ((dx < 0 && dy != 0) && abs(dx) + abs(dy) < 12)
          {
            return true;
          }
        }
      }
      else if ((*chrflag && !*invflag) || (!*chrflag && *invflag))
      {
        return panelScreenCheck[std::make_pair(dx, dy)];
      }
      return false;
    };

    auto message = std::make_unique<dapi::message::Message>();
    auto update = message->mutable_frameupdate();

    data->player = *myplr;
    update->set_player(data->player);



    data->stextflag = *stextflag;
    update->set_stextflag(data->stextflag);
    data->pauseMode = *PauseMode;
    update->set_pausemode(data->pauseMode);
    if (gmenu_is_on())
      data->menuOpen = true;
    else
      data->menuOpen = false;
    update->set_menuopen(data->menuOpen);
    data->pcurs = *pcurs;
    update->set_cursor(*pcurs);
    data->chrflag = *chrflag;
    update->set_chrflag(*chrflag);
    data->invflag = *invflag;
    update->set_invflag(*invflag);
    data->qtextflag = *qtextflag;
    update->set_qtextflag(*qtextflag);
    if (!*setlevel)
      data->currlevel = static_cast<int>(*currlevel);
    else
      data->currlevel = static_cast<int>(*setlvlnum);
    update->set_currlevel(*currlevel);
    data->setlevel = static_cast<bool>(*setlevel);
    update->set_setlevel(*setlevel);
    if (*qtextflag)
      update->set_qtext(*qtextptr);
    else
      update->set_qtext("");
    update->set_fps(FPS);
    if (*gbMaxPlayers == 1)
      update->set_gamemode(0);
    else
      update->set_gamemode(1);

    int range = 10;
    if (ScrollInfo->_sdir != 0) {
      range = 11;
    }

    for (int x = 0; x < 112; x++) {
      for (int y = 0; y < 112; y++) {
        if (isOnScreen(x, y))
        {
          auto dpiece = update->add_dpiece();
          dpiece->set_type((*dPiece)[x][y]);
          dpiece->set_solid(static_cast<bool>(nSolidTable[dpiece->type()]));
          dpiece->set_x(x);
          dpiece->set_y(y);
          dpiece->set_stopmissile(static_cast<bool>(nMissileTable[dpiece->type()]));
        }
      }
    }

    for (int i = 0; i < *numtrigs; i++)
    {
      if (isOnScreen(trigs[i]._tx, trigs[i]._ty))
      {
        auto trigger = update->add_triggerdata();
        trigger->set_lvl(trigs[i]._tlvl);
        trigger->set_x(trigs[i]._tx);
        trigger->set_y(trigs[i]._ty);
        trigger->set_type(trigs[i]._tmsg);
      }
    }

    for (int i = 0; i < MAXQUESTS; i++)
    {
      auto quest = update->add_questdata();
      quest->set_id(i);
      if (quests[i]._qactive == 2)
        quest->set_state(quests[i]._qactive);
      else
        quest->set_state(0);

      if (*currlevel == quests[i]._qlevel &&
        quests[i]._qslvl != 0 &&
        static_cast<DiabloInternal::QuestState>(quests[i]._qactive) != DiabloInternal::QuestState::NOTAVAIL)
      {
        auto trigger = update->add_triggerdata();
        trigger->set_lvl(quests[i]._qslvl);
        trigger->set_x(quests[i]._qtx);
        trigger->set_y(quests[i]._qty);
        trigger->set_type(1029);
      }
    }

    data->storeList.clear();
    if (*stextflag)
    {
      for (int i = 0; i < 24; i++)
      {
        if (stext[i]._ssel)
        {
          if (!strcmp(stext[i]._sstr, "Talk to Cain") || !strcmp(stext[i]._sstr, "Talk to Farnham") || !strcmp(stext[i]._sstr, "Talk to Pepin") || !strcmp(stext[i]._sstr, "Talk to Gillian") || !strcmp(stext[i]._sstr, "Talk to Ogden") || !strcmp(stext[i]._sstr, "Talk to Griswold") || !strcmp(stext[i]._sstr, "Talk to Adria") || !strcmp(stext[i]._sstr, "Talk to Wirt"))
            data->storeList.push_back(StoreOption::TALK);
          else if (!strcmp(stext[i]._sstr, "Identify an item"))
            data->storeList.push_back(StoreOption::IDENTIFYANITEM);
          else if (!strcmp(stext[i]._sstr, "Say goodbye") || !strcmp(stext[i]._sstr, "Say Goodbye") || !strcmp(stext[i]._sstr, "Leave Healer's home") || !strcmp(stext[i]._sstr, "Leave the shop") || !strcmp(stext[i]._sstr, "Leave the shack") || !strcmp(stext[i]._sstr, "Leave the tavern"))
            data->storeList.push_back(StoreOption::EXIT);
          else if (!strcmp(stext[i]._sstr, "Receive healing"))
            data->storeList.push_back(StoreOption::HEAL);
          else if (!strcmp(stext[i]._sstr, "Buy items"))
            data->storeList.push_back(StoreOption::BUYITEMS);
          else if (!strcmp(stext[i]._sstr, "What have you got?"))
            data->storeList.push_back(StoreOption::WIRTPEEK);
          else if (!strcmp(stext[i]._sstr, "Buy basic items"))
            data->storeList.push_back(StoreOption::BUYBASIC);
          else if (!strcmp(stext[i]._sstr, "Buy premium items"))
            data->storeList.push_back(StoreOption::BUYPREMIUM);
          else if (!strcmp(stext[i]._sstr, "Sell items"))
            data->storeList.push_back(StoreOption::SELL);
          else if (!strcmp(stext[i]._sstr, "Repair items"))
            data->storeList.push_back(StoreOption::REPAIR);
          else if (!strcmp(stext[i]._sstr, "Recharge staves"))
            data->storeList.push_back(StoreOption::RECHARGE);
        }
      }

      switch (static_cast<TalkID>(*stextflag))
      {
      case TalkID::HBUY:
      case TalkID::IDSHOW:
      case TalkID::NOMONEY:
      case TalkID::NOROOM:
      case TalkID::SBUY:
      case TalkID::SIDENTIFY:
      case TalkID::SPBUY:
      case TalkID::SREPAIR:
      case TalkID::SSELL:
      case TalkID::WBUY:
      case TalkID::WRECHARGE:
      case TalkID::WSELL:
      case TalkID::GOSSIP:
        data->storeList.push_back(StoreOption::BACK);
        break;
      default:
        break;
      }
    }

    for (auto& option : data->storeList)
      update->add_storeoption(static_cast<int>(option));

    data->groundItems.clear();

    for (int i = 0; i < 4; i++)
    {
      auto playerData = update->add_playerdata();

      data->playerList[i].InvBody.clear();

      data->playerList[i].pnum = i;
      playerData->set_pnum(i);

      memcpy(data->playerList[i]._pName, plr[i]._pName, 32);
      playerData->set__pname(data->playerList[i]._pName);

      for (int j = 0; j < MAXINV; j++)
        data->playerList[i].InvList[j] = -1;

      if (*myplr == i)
      {
        data->playerList[i]._pmode = plr[i]._pmode;
        data->playerList[i].plrlevel = plr[i].plrlevel;
        data->playerList[i]._px = plr[i]._px;
        data->playerList[i]._py = plr[i]._py;
        data->playerList[i]._pfutx = plr[i]._pfutx;
        data->playerList[i]._pfuty = plr[i]._pfuty;
        data->playerList[i]._pdir = plr[i]._pdir;

        data->playerList[i]._pRSpell = plr[i]._pRSpell;
        data->playerList[i]._pRSplType = plr[i]._pRSplType;


        memcpy(data->playerList[i]._pSplLvl, plr[i]._pSplLvl, sizeof(data->playerList[i]._pSplLvl));
        data->playerList[i]._pMemSpells = plr[i]._pMemSpells;
        data->playerList[i]._pAblSpells = plr[i]._pAblSpells;
        data->playerList[i]._pScrlSpells = plr[i]._pScrlSpells;

        data->playerList[i]._pClass = plr[i]._pClass;

        data->playerList[i]._pStrength = plr[i]._pStrength;
        data->playerList[i]._pBaseStr = plr[i]._pBaseStr;
        data->playerList[i]._pMagic = plr[i]._pMagic;
        data->playerList[i]._pBaseMag = plr[i]._pBaseMag;
        data->playerList[i]._pDexterity = plr[i]._pDexterity;
        data->playerList[i]._pBaseDex = plr[i]._pBaseDex;
        data->playerList[i]._pVitality = plr[i]._pVitality;
        data->playerList[i]._pBaseVit = plr[i]._pBaseVit;

        data->playerList[i]._pStatPts = plr[i]._pStatPts;

        data->playerList[i]._pDamageMod = plr[i]._pDamageMod;

        data->playerList[i]._pHitPoints = plr[i]._pHitPoints;
        data->playerList[i]._pMaxHP = plr[i]._pMaxHP;
        data->playerList[i]._pMana = plr[i]._pMana;
        data->playerList[i]._pMaxMana = plr[i]._pMaxMana;
        data->playerList[i]._pLevel = plr[i]._pLevel;
        data->playerList[i]._pExperience = plr[i]._pExperience;

        data->playerList[i]._pArmorClass = plr[i]._pArmorClass;

        data->playerList[i]._pMagResist = plr[i]._pMagResist;
        data->playerList[i]._pFireResist = plr[i]._pFireResist;
        data->playerList[i]._pLightResist = plr[i]._pLghtResist;

        data->playerList[i]._pGold = plr[i]._pGold;

        for (int j = 0; j < NUM_INVLOC; j++)
        {
          if (plr[i].InvBody[j]._itype == -1)
          {
            data->playerList[i].InvBody[j] = -1;
            continue;
          }

          int itemID = data->itemList.size();
          for (int k = 0; k < data->itemList.size(); k++)
          {
            if (data->itemList[k].compare(plr[i].InvBody[j]))
            {
              itemID = k;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &plr[i].InvBody[j]);
          data->playerList[i].InvBody[j] = itemID;

        }
        bool used[40] = { false };
        for (int j = 0; j < MAXINV; j++)
        {
          auto index = plr[i].InvGrid[j];
          if (index != 0)
          {
            int itemID = data->itemList.size();
            for (int k = 0; k < data->itemList.size(); k++)
            {
              if (data->itemList[k].compare(plr[i].InvList[abs(index) - 1]))
              {
                itemID = k;
                break;
              }
            }
            data->playerList[i].InvGrid[j] = itemID;
            if (!used[abs(index) - 1])
            {
              if (itemID == data->itemList.size())
                data->itemList.push_back(ItemData{});
              fullFillItemInfo(itemID, &plr[i].InvList[abs(index) - 1]);
              used[abs(index) - 1] = true;
              data->playerList[i].InvList[abs(index) - 1] = itemID;
            }
          }
          else
            data->playerList[i].InvGrid[j] = -1;
        }
        for (int j = 0; j < MAXSPD; j++)
        {
          if (plr[i].SpdList[j]._itype == -1)
          {
            data->playerList[i].SpdList[j] = -1;
            continue;
          }

          int itemID = data->itemList.size();
          for (int k = 0; k < data->itemList.size(); k++)
          {
            if (data->itemList[k].compare(plr[i].SpdList[j]))
            {
              itemID = k;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &plr[i].SpdList[j]);
          data->playerList[i].SpdList[j] = itemID;
        }
        if (*pcurs < 12)
          data->playerList[i].HoldItem = -1;
        else
        {
          int itemID = data->itemList.size();
          for (int j = 0; j < data->itemList.size(); j++)
          {
            if (data->itemList[j].compare(plr[i].HoldItem))
            {
              itemID = j;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          partialFillItemInfo(itemID, &plr[i].HoldItem);
          data->playerList[i].HoldItem = itemID;
        }

        data->playerList[i]._pIMinDam = plr[i]._pIMinDam;
        data->playerList[i]._pIMaxDam = plr[i]._pIMaxDam;
        data->playerList[i]._pIBonusDam = plr[i]._pIBonusDam;
        data->playerList[i]._pIAC = plr[i]._pIAC;
        data->playerList[i]._pIBonusToHit = plr[i]._pIBonusToHit;
        data->playerList[i]._pIBonusAC = plr[i]._pIBonusAC;
        data->playerList[i]._pIBonusDamMod = plr[i]._pIBonusDamMod;
      }
      else if ((*dFlags)[plr[i]._px][plr[i]._py] & 0x40)
      {
        data->playerList[i]._pmode = plr[i]._pmode;
        data->playerList[i].plrlevel = plr[i].plrlevel;
        data->playerList[i]._px = plr[i]._px;
        data->playerList[i]._py = plr[i]._py;
        data->playerList[i]._pfutx = plr[i]._pfutx;
        data->playerList[i]._pfuty = plr[i]._pfuty;
        data->playerList[i]._pdir = plr[i]._pdir;

        data->playerList[i]._pRSpell = -1;
        data->playerList[i]._pRSplType = 4;

        memset(data->playerList[i]._pSplLvl, 0, 64);
        data->playerList[i]._pMemSpells = -1;
        data->playerList[i]._pAblSpells = -1;
        data->playerList[i]._pScrlSpells = -1;

        data->playerList[i]._pClass = plr[i]._pClass;

        data->playerList[i]._pStrength = -1;
        data->playerList[i]._pBaseStr = -1;
        data->playerList[i]._pMagic = -1;
        data->playerList[i]._pBaseMag = -1;
        data->playerList[i]._pDexterity = -1;
        data->playerList[i]._pBaseDex = -1;
        data->playerList[i]._pVitality = -1;
        data->playerList[i]._pBaseVit = -1;

        data->playerList[i]._pStatPts = -1;

        data->playerList[i]._pDamageMod = -1;

        data->playerList[i]._pHitPoints = plr[i]._pHitPoints;
        data->playerList[i]._pMaxHP = plr[i]._pMaxHP;
        data->playerList[i]._pMana = -1;
        data->playerList[i]._pMaxMana = -1;
        data->playerList[i]._pLevel = plr[i]._pLevel;
        data->playerList[i]._pExperience = -1;

        data->playerList[i]._pArmorClass = -1;

        data->playerList[i]._pMagResist = -1;
        data->playerList[i]._pFireResist = -1;
        data->playerList[i]._pLightResist = -1;

        data->playerList[i]._pGold = -1;

        for (int j = 0; j < NUM_INVLOC; j++)
          data->playerList[i].InvBody[j] = -1;
        for (int j = 0; j < MAXINV; j++)
          data->playerList[i].InvGrid[j] = -1;
        for (int j = 0; j < MAXSPD; j++)
          data->playerList[i].SpdList[j] = -1;
        data->playerList[i].HoldItem = -1;

        data->playerList[i]._pIMinDam = -1;
        data->playerList[i]._pIMaxDam = -1;
        data->playerList[i]._pIBonusDam = -1;
        data->playerList[i]._pIAC = -1;
        data->playerList[i]._pIBonusToHit = -1;
        data->playerList[i]._pIBonusAC = -1;
        data->playerList[i]._pIBonusDamMod = -1;
      }
      else
      {
        data->playerList[i]._pmode = 0;
        data->playerList[i].plrlevel = -1;
        data->playerList[i]._px = -1;
        data->playerList[i]._py = -1;
        data->playerList[i]._pfutx = -1;
        data->playerList[i]._pfuty = -1;
        data->playerList[i]._pdir = -1;

        data->playerList[i]._pRSpell = -1;
        data->playerList[i]._pRSplType = -1;

        memset(data->playerList[i]._pSplLvl, 0, 64);
        data->playerList[i]._pMemSpells = -1;
        data->playerList[i]._pAblSpells = -1;
        data->playerList[i]._pScrlSpells = -1;

        data->playerList[i]._pClass = -1;

        data->playerList[i]._pStrength = -1;
        data->playerList[i]._pBaseStr = -1;
        data->playerList[i]._pMagic = -1;
        data->playerList[i]._pBaseMag = -1;
        data->playerList[i]._pDexterity = -1;
        data->playerList[i]._pBaseDex = -1;
        data->playerList[i]._pVitality = -1;
        data->playerList[i]._pBaseVit = -1;

        data->playerList[i]._pStatPts = -1;

        data->playerList[i]._pDamageMod = -1;

        data->playerList[i]._pHitPoints = -1;
        data->playerList[i]._pMaxHP = -1;
        data->playerList[i]._pMana = -1;
        data->playerList[i]._pMaxMana = -1;
        data->playerList[i]._pLevel = plr[i]._pLevel;
        data->playerList[i]._pExperience = -1;

        data->playerList[i]._pArmorClass = -1;

        data->playerList[i]._pMagResist = -1;
        data->playerList[i]._pFireResist = -1;
        data->playerList[i]._pLightResist = -1;
        data->playerList[i]._pIBonusToHit = -1;

        data->playerList[i]._pGold = -1;
        for (int j = 0; j < NUM_INVLOC; j++)
          data->playerList[i].InvBody[j] = -1;
        for (int j = 0; j < MAXINV; j++)
          data->playerList[i].InvGrid[j] = -1;
        for (int j = 0; j < MAXSPD; j++)
          data->playerList[i].SpdList[j] = -1;
        data->playerList[i].HoldItem = -1;

        data->playerList[i]._pIMinDam = -1;
        data->playerList[i]._pIMaxDam = -1;
        data->playerList[i]._pIBonusDam = -1;
        data->playerList[i]._pIAC = -1;
        data->playerList[i]._pIBonusToHit = -1;
        data->playerList[i]._pIBonusAC = -1;
        data->playerList[i]._pIBonusDamMod = -1;
      }

      playerData->set__pmode(data->playerList[i]._pmode);
      playerData->set_plrlevel(data->playerList[i].plrlevel);
      playerData->set__px(data->playerList[i]._px);
      playerData->set__py(data->playerList[i]._py);
      playerData->set__pfutx(data->playerList[i]._pfutx);
      playerData->set__pfuty(data->playerList[i]._pfuty);
      playerData->set__pdir(data->playerList[i]._pdir);

      playerData->set__prspell(data->playerList[i]._pRSpell);
      playerData->set__prspltype(data->playerList[i]._pRSplType);

      for (int j = 0; j < 64; j++)
        playerData->add__pspllvl(data->playerList[i]._pSplLvl[j]);
      playerData->set__pmemspells(data->playerList[i]._pMemSpells);
      playerData->set__pablspells(data->playerList[i]._pAblSpells);
      playerData->set__pscrlspells(data->playerList[i]._pScrlSpells);

      playerData->set__pclass(data->playerList[i]._pClass);

      playerData->set__pstrength(data->playerList[i]._pStrength);
      playerData->set__pbasestr(data->playerList[i]._pBaseStr);
      playerData->set__pmagic(data->playerList[i]._pMagic);
      playerData->set__pbasemag(data->playerList[i]._pBaseMag);
      playerData->set__pdexterity(data->playerList[i]._pDexterity);
      playerData->set__pbasedex(data->playerList[i]._pBaseDex);
      playerData->set__pvitality(data->playerList[i]._pVitality);
      playerData->set__pbasevit(data->playerList[i]._pBaseVit);

      playerData->set__pstatpts(data->playerList[i]._pStatPts);

      playerData->set__pdamagemod(data->playerList[i]._pDamageMod);

      playerData->set__phitpoints(data->playerList[i]._pHitPoints);
      playerData->set__pmaxhp(data->playerList[i]._pMaxHP);
      playerData->set__pmana(data->playerList[i]._pMana);
      playerData->set__pmaxmana(data->playerList[i]._pMaxMana);
      playerData->set__plevel(data->playerList[i]._pLevel);
      playerData->set__pexperience(data->playerList[i]._pExperience);

      playerData->set__parmorclass(data->playerList[i]._pArmorClass);

      playerData->set__pmagresist(data->playerList[i]._pMagResist);
      playerData->set__pfireresist(data->playerList[i]._pFireResist);
      playerData->set__plightresist(data->playerList[i]._pLightResist);

      playerData->set__pgold(data->playerList[i]._pGold);
      for (int j = 0; j < NUM_INVLOC; j++)
        playerData->add_invbody(data->playerList[i].InvBody[j]);
      for (int j = 0; j < MAXINV; j++)
        playerData->add_invlist(data->playerList[i].InvList[j]);
      for (int j = 0; j < MAXINV; j++)
        playerData->add_invgrid(data->playerList[i].InvGrid[j]);
      for (int j = 0; j < MAXSPD; j++)
        playerData->add_spdlist(data->playerList[i].SpdList[j]);
      playerData->set_holditem(data->playerList[i].HoldItem);

      playerData->set__pimindam(data->playerList[i]._pIMinDam);
      playerData->set__pimaxdam(data->playerList[i]._pIMaxDam);
      playerData->set__pibonusdam(data->playerList[i]._pIBonusDam);
      playerData->set__piac(data->playerList[i]._pIAC);
      playerData->set__pibonustohit(data->playerList[i]._pIBonusToHit);
      playerData->set__pibonusac(data->playerList[i]._pIBonusAC);
      playerData->set__pibonusdammod(data->playerList[i]._pIBonusDamMod);
    }

    auto emptyFillItemInfo = [&](int itemID, DiabloInternal::ItemStruct* item) {

      data->itemList[itemID].ID = itemID;
      data->itemList[itemID]._iSeed = item->_iSeed;
      data->itemList[itemID]._iCreateInfo = item->_iCreateInfo;
      data->itemList[itemID]._itype = item->_itype;
      data->itemList[itemID]._ix = -1;
      data->itemList[itemID]._iy = -1;

      data->itemList[itemID]._iIdentified = -1;
      data->itemList[itemID]._iMagical = -1;
      strcpy_s(data->itemList[itemID]._iName, "");
      strcpy_s(data->itemList[itemID]._iIName, "");
      data->itemList[itemID]._iFlags = -1;
      data->itemList[itemID]._iPrePower = -1;
      data->itemList[itemID]._iSufPower = -1;
      data->itemList[itemID]._iPLDam = -1;
      data->itemList[itemID]._iPLToHit = -1;
      data->itemList[itemID]._iPLAC = -1;
      data->itemList[itemID]._iPLStr = -1;
      data->itemList[itemID]._iPLMag = -1;
      data->itemList[itemID]._iPLDex = -1;
      data->itemList[itemID]._iPLVit = -1;
      data->itemList[itemID]._iPLFR = -1;
      data->itemList[itemID]._iPLLR = -1;
      data->itemList[itemID]._iPLMR = -1;
      data->itemList[itemID]._iPLMana = -1;
      data->itemList[itemID]._iPLHP = -1;
      data->itemList[itemID]._iPLDamMod = -1;
      data->itemList[itemID]._iPLGetHit = -1;
      data->itemList[itemID]._iPLLight = -1;
      data->itemList[itemID]._iSplLvlAdd = -1;
      data->itemList[itemID]._iFMinDam = -1;
      data->itemList[itemID]._iFMaxDam = -1;
      data->itemList[itemID]._iLMinDam = -1;
      data->itemList[itemID]._iLMaxDam = -1;
      data->itemList[itemID]._iClass = -1;
      data->itemList[itemID]._ivalue = -1;
      data->itemList[itemID]._iMinDam = -1;
      data->itemList[itemID]._iMaxDam = -1;
      data->itemList[itemID]._iAC = -1;
      data->itemList[itemID]._iMiscId = -1;
      data->itemList[itemID]._iSpell = -1;

      data->itemList[itemID]._iCharges = -1;
      data->itemList[itemID]._iMaxCharges = -1;

      data->itemList[itemID]._iDurability = -1;
      data->itemList[itemID]._iMaxDur = -1;

      data->itemList[itemID]._iMinStr = -1;
      data->itemList[itemID]._iMinMag = -1;
      data->itemList[itemID]._iMinDex = -1;
      data->itemList[itemID]._iStatFlag = -1;
      data->itemList[itemID].IDidx = item->IDidx;
    };

    for (int i = 0; i < *numitems; i++)
    {
      int itemID = data->itemList.size();
      for (int j = 0; j < data->itemList.size(); j++)
      {
        if (data->itemList[j].compare(item[itemactive[i]]))
        {
          itemID = j;
          break;
        }
      }
      if (itemID == data->itemList.size())
        data->itemList.push_back(ItemData{});
      int dx = plr[*myplr]._px - item[itemactive[i]]._ix;
      int dy = plr[*myplr]._py - item[itemactive[i]]._iy;
      if (dy > 0)
      {
        if (dx < 1 && abs(dx) + abs(dy) < 11)
        {
          partialFillItemInfo(itemID, &item[itemactive[i]]);
          data->groundItems.push_back(itemID);
        }
        else if (dx > 0 && abs(dx) + abs(dy) < 12)
        {
          partialFillItemInfo(itemID, &item[itemactive[i]]);
          data->groundItems.push_back(itemID);
        }
        else
          emptyFillItemInfo(itemID, &item[itemactive[i]]);
      }
      else
      {
        if ((dx > -1 || dy == 0) && abs(dx) + abs(dy) < 11)
        {
          partialFillItemInfo(itemID, &item[itemactive[i]]);
          data->groundItems.push_back(itemID);
        }
        else if ((dx < 0 && dy != 0) && abs(dx) + abs(dy) < 12)
        {
          partialFillItemInfo(itemID, &item[itemactive[i]]);
          data->groundItems.push_back(itemID);
        }
        else
          emptyFillItemInfo(itemID, &item[itemactive[i]]);
      }

    }

    data->storeItems.clear();
    switch (static_cast<TalkID>(*stextflag))
    {
    case TalkID::SIDENTIFY:
    case TalkID::WSELL:
    case TalkID::WRECHARGE:
    case TalkID::SSELL:
    case TalkID::SREPAIR:
      for (int i = 0; i < 48; i++)
      {
        if (storehold[i]._itype != -1)
        {
          int itemID = data->itemList.size();
          for (auto& item : data->itemList)
          {
            if (item.compare(storehold[i]))
            {
              itemID = item.ID;
              break;
            }
          }
          if (itemID == data->itemList.size())
          {
            data->itemList.push_back(ItemData{});
            fullFillItemInfo(itemID, &storehold[i]);
          }
          data->itemList[itemID]._ivalue = storehold[i]._ivalue;
          if (storehold[i]._ivalue != 0)
          {
            data->storeItems.push_back(itemID);
            update->add_storeitems(itemID);
          }
        }
      }
      break;
    case TalkID::WBUY:
      for (int i = 0; i < 20; i++)
      {
        if (witchitem[i]._itype != -1)
        {
          int itemID = data->itemList.size();
          for (int j = 0; j < data->itemList.size(); j++)
          {
            if (data->itemList[j].compare(witchitem[i]))
            {
              itemID = j;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &witchitem[i]);
          data->itemList[itemID]._ivalue = witchitem[i]._ivalue;
          data->storeItems.push_back(itemID);
          update->add_storeitems(itemID);
        }
      }
      break;
    case TalkID::SBUY:
      for (int i = 0; i < 20; i++)
      {
        if (smithitem[i]._itype != -1)
        {
          int itemID = data->itemList.size();
          for (int j = 0; j < data->itemList.size(); j++)
          {
            if (data->itemList[j].compare(smithitem[i]))
            {
              itemID = j;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &smithitem[i]);
          data->itemList[itemID]._ivalue = smithitem[i]._ivalue;
          data->storeItems.push_back(itemID);
          update->add_storeitems(itemID);
        }
      }
      break;
    case TalkID::HBUY:
      for (int i = 0; i < 20; i++)
      {
        if (healitem[i]._itype != -1)
        {
          int itemID = data->itemList.size();
          for (int j = 0; j < data->itemList.size(); j++)
          {
            if (data->itemList[j].compare(healitem[i]))
            {
              itemID = j;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &healitem[i]);
          data->itemList[itemID]._ivalue = healitem[i]._ivalue;
          data->storeItems.push_back(itemID);
          update->add_storeitems(itemID);
        }
      }
      break;
    case TalkID::SPBUY:
      for (int i = 0; i < 6; i++)
      {
        if (premiumitem[i]._itype != -1)
        {
          int itemID = data->itemList.size();
          for (int j = 0; j < data->itemList.size(); j++)
          {
            if (data->itemList[j].compare(premiumitem[i]))
            {
              itemID = j;
              break;
            }
          }
          if (itemID == data->itemList.size())
            data->itemList.push_back(ItemData{});
          fullFillItemInfo(itemID, &premiumitem[i]);
          data->itemList[itemID]._ivalue = premiumitem[i]._iIvalue;
          data->storeItems.push_back(itemID);
          update->add_storeitems(itemID);
        }
      }
    }

    

    if (*currlevel != 0)
    {
      for (auto &townerData : data->townerList)
      {
        strcpy_s(townerData._tName, "");
        townerData._tx = -1;
        townerData._ty = -1;
      }
    }
    else
    {
      for (auto i = 0; i < *numtowners; i++)
      {
        auto townerID = data->townerList.size();
        for (int j = 0; j < data->townerList.size(); j++)
        {
          if (data->townerList[j]._ttype == static_cast<TalkerType>(towner[i]._ttype))
          {
            townerID = j;
            break;
          }
        }
        if (townerID == data->townerList.size())
          data->townerList.push_back(TownerData{});
        data->townerList[townerID].ID = townerID;
        if (isOnScreen(towner[i]._tx, towner[i]._ty))
        {
          data->townerList[townerID]._ttype = static_cast<TalkerType>(towner[i]._ttype);
          data->townerList[townerID]._tx = towner[i]._tx;
          data->townerList[townerID]._ty = towner[i]._ty;
          strcpy_s(data->townerList[townerID]._tName, towner[i]._tName);
        }
        else
        {
          data->townerList[townerID]._ttype = static_cast<TalkerType>(towner[i]._ttype);
          data->townerList[townerID]._tx = -1;
          data->townerList[townerID]._ty = -1;
          strcpy_s(data->townerList[townerID]._tName, "");
        }
      }
    }

    for (auto& townie : data->townerList)
    {
      auto townerData = update->add_townerdata();
      townerData->set_id(townie.ID);
      if (townie._tx != -1)
        townerData->set__ttype(static_cast<int>(townie._ttype));
      else
        townerData->set__ttype(static_cast<int>(TalkerType::NONE));
      townerData->set__tx(townie._tx);
      townerData->set__ty(townie._ty);
      townerData->set__tname(townie._tName);
    }

    for (auto& item : data->itemList)
    {
      auto itemData = update->add_itemdata();
      itemData->set_id(item.ID);
      itemData->set__itype(item._itype);
      itemData->set__ix(item._ix);
      itemData->set__iy(item._iy);
      itemData->set__iidentified(item._iIdentified);
      itemData->set__imagical(item._iMagical);
      itemData->set__iname(item._iName);
      itemData->set__iiname(item._iIName);
      itemData->set__iclass(item._iClass);
      itemData->set__icurs(item._iCurs);
      itemData->set__ivalue(item._ivalue);
      itemData->set__imindam(item._iMinDam);
      itemData->set__imaxdam(item._iMaxDam);
      itemData->set__iac(item._iAC);
      itemData->set__iflags(item._iFlags);
      itemData->set__imiscid(item._iMiscId);
      itemData->set__ispell(item._iSpell);
      itemData->set__icharges(item._iCharges);
      itemData->set__imaxcharges(item._iMaxCharges);
      itemData->set__idurability(item._iDurability);
      itemData->set__imaxdur(item._iMaxDur);
      itemData->set__ipldam(item._iPLDam);
      itemData->set__ipltohit(item._iPLToHit);
      itemData->set__iplac(item._iPLAC);
      itemData->set__iplstr(item._iPLStr);
      itemData->set__iplmag(item._iPLMag);
      itemData->set__ipldex(item._iPLDex);
      itemData->set__iplvit(item._iPLVit);
      itemData->set__iplfr(item._iPLFR);
      itemData->set__ipllr(item._iPLLR);
      itemData->set__iplmr(item._iPLMR);
      itemData->set__iplmana(item._iPLMana);
      itemData->set__iplhp(item._iPLHP);
      itemData->set__ipldammod(item._iPLDamMod);
      itemData->set__iplgethit(item._iPLGetHit);
      itemData->set__ipllight(item._iPLLight);
      itemData->set__ispllvladd(item._iSplLvlAdd);
      itemData->set__ifmindam(item._iFMinDam);
      itemData->set__ifmaxdam(item._iFMaxDam);
      itemData->set__ilmindam(item._iLMinDam);
      itemData->set__ilmaxdam(item._iLMaxDam);
      itemData->set__iprepower(item._iPrePower);
      itemData->set__isufpower(item._iSufPower);
      itemData->set__iminstr(item._iMinStr);
      itemData->set__iminmag(item._iMinMag);
      itemData->set__imindex(item._iMinDex);
      itemData->set__istatflag(item._iStatFlag);
      itemData->set_ididx(item.IDidx);
    }
    for (auto& itemID : data->groundItems)
      update->add_grounditemid(itemID);

    for (int i = 0; i < *nummonsters; i++)
    {
      if (isOnScreen(monster[monstactive[i]]._mx, monster[monstactive[i]]._my) && monster[monstactive[i]]._mhitpoints >> 6 > 0 && (*dFlags)[monster[monstactive[i]]._mx][monster[monstactive[i]]._my] & 0x40 && !(monster[monstactive[i]]._mFlags & 0x01))
      {
        auto m = update->add_monsterdata();
        m->set_index(monstactive[i]);
        m->set_x(monster[monstactive[i]]._mx);
        m->set_y(monster[monstactive[i]]._my);
        m->set_futx(monster[monstactive[i]]._mfutx);
        m->set_futy(monster[monstactive[i]]._mfuty);
        m->set_type(monster[monstactive[i]].MType->mtype);
        m->set_name(monster[monstactive[i]].mName);
        m->set_unique(static_cast<bool>(monster[monstactive[i]]._uniqtype));
      }
    }

    auto fillObject = [&](int index, DiabloInternal::ObjectStruct& ob) {
      auto o = update->add_objectdata();
      o->set_type(ob._otype);
      o->set_x(ob._ox);
      o->set_y(ob._oy);
      o->set_shrinetype(-1);
      o->set_solid(ob._oSolidFlag);
      o->set_doorstate(-1);
      o->set_selectable(ob._oSelFlag);
      o->set_index(index);
      switch (static_cast<DiabloInternal::ObjectID>(ob._otype))
      {
      case DiabloInternal::ObjectID::BARRELEX:
        o->set_type(static_cast<int>(DiabloInternal::ObjectID::BARREL));
        break;
      case DiabloInternal::ObjectID::SHRINEL:
      case DiabloInternal::ObjectID::SHRINER:
        if (ob._oSelFlag)
          o->set_shrinetype(ob._oVar1);
        break;
      case DiabloInternal::ObjectID::L1LDOOR:
      case DiabloInternal::ObjectID::L1RDOOR:
      case DiabloInternal::ObjectID::L2LDOOR:
      case DiabloInternal::ObjectID::L2RDOOR:
      case DiabloInternal::ObjectID::L3LDOOR:
      case DiabloInternal::ObjectID::L3RDOOR:
        o->set_doorstate(ob._oVar4);
        break;
      }
      if (static_cast<DiabloInternal::PlayerClasses>(plr[*myplr]._pClass) == DiabloInternal::PlayerClasses::ROGUE)
        o->set_trapped(ob._oTrapFlag);
      else
        o->set_trapped(false);
    };

    auto fillMissile = [&](DiabloInternal::MissileStruct& ms) {
      auto m = update->add_missiledata();
      m->set_type(ms._mitype);
      m->set_x(ms._mix);
      m->set_y(ms._miy);
      m->set_xvel(ms._mixvel);
      m->set_yvel(ms._miyvel);
      if (isOnScreen(ms._misx, ms._misy))
      {
        m->set_sx(ms._misx);
        m->set_sy(ms._misy);
      }
      else
      {
        m->set_sx(-1);
        m->set_sy(-1);
      }
    };

    if (plr[*myplr].plrlevel != 0)
    {
      for (int i = 0; i < *nobjects; i++)
      {
        if (isOnScreen(object[objectactive[i]]._ox, object[objectactive[i]]._oy) &&
            (*dObject)[object[objectactive[i]]._ox][object[objectactive[i]]._oy] == objectactive[i] + 1)
        {
          fillObject(objectactive[i], object[objectactive[i]]);
          /*switch (static_cast<DiabloInternal::ObjectID>(object[i]._otype))
          {
          case DiabloInternal::ObjectID::BARREL:
          case DiabloInternal::ObjectID::BARRELEX:
            if (object[i]._oSolidFlag)
              fillObject(i, object[i]);
            break;
          default:
            fillObject(i, object[i]);
            break;
          }*/
        }
      }

      for (int i = 0; i < *nummissiles; i++)
      {
        if (isOnScreen(missile[missileactive[i]]._mix, missile[missileactive[i]]._miy))
          fillMissile(missile[missileactive[i]]);
      }
    }

    auto WarpDropX = reinterpret_cast<int(*)>(0x4A1ABC);
    auto WarpDropY = reinterpret_cast<int(*)>(0x4A1ACC);

    for (int i = 0; i < 4; i++)
    {
      if (portal[i].open && (portal[i].level == plr[*myplr].plrlevel) &&
        isOnScreen(portal[i].x, portal[i].y))
      {
        auto tp = update->add_portaldata();
        tp->set_x(portal[i].x);
        tp->set_y(portal[i].y);
        tp->set_player(i);
      }
      else if (portal[i].open && 0 == plr[*myplr].plrlevel &&
        isOnScreen(WarpDropX[i], WarpDropY[i]))
      {
        auto tp = update->add_portaldata();
        tp->set_x(WarpDropX[i]);
        tp->set_y(WarpDropY[i]);
        tp->set_player(i);
      }
    }

    protoClient.queueMessage(std::move(message));
  }

  bool Server::isOnScreen(int x, int y)
  {
    auto chrflag = reinterpret_cast<BOOL(*)>(0x4B896C);

    bool returnValue = false;
    int dx = data->playerList[data->player]._px - x;
    int dy = data->playerList[data->player]._py - y;
    if (!*chrflag)
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
    else if (*chrflag)
    {
      returnValue = panelScreenCheck[std::make_pair(dx, dy)];
    }
    return returnValue;
  }

  bool Server::OKToAct()
  {
    return !data->stextflag && data->pcurs == 1 && !data->qtextflag;
  }

  void Server::move(int x, int y)
  {
    auto NetSendCmdLoc = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);
    NetSendCmdLoc(1u, static_cast<unsigned char>(CommandType::WALKXY), x, y);
  }

  void Server::talk(int x, int y)
  {
    auto towner = reinterpret_cast<DiabloInternal::TownerStruct(*)>(0x6AAC38);
    auto numtowners = reinterpret_cast<int(*)>(0x6AAC20);
    auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
    int index;
    for (index = 0; index < *numtowners; index++)
    {
      if (towner[index]._tx == x && towner[index]._ty == y)
        break;
    }
    if (index != *numtowners)
      NetSendCmdLocParam1(1u, static_cast<unsigned char>(CommandType::TALKXY), x, y, index);
  }

  void Server::selectStoreOption(StoreOption option)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto stextlhold = reinterpret_cast<int(*)>(0x69F110);
    auto talker = reinterpret_cast<int(*)>(0x69FB38);
    auto stextshold = reinterpret_cast<int(*)>(0x69F288);
    auto gossipstart = reinterpret_cast<int(*)>(0x6A4EF0);
    auto gossipend = reinterpret_cast<int(*)>(0x6A8A30);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto drawhpflag = reinterpret_cast<BOOL(*)>(0x4B84D8);
    switch (option)
    {
    case StoreOption::TALK:
      switch (static_cast<TalkID>(*stextflag))
      {
      case TalkID::WITCH:
        PlaySFX(70);
        *stextlhold = 12;
        *talker = 6;
        *stextshold = static_cast<int>(TalkID::WITCH);
        *gossipstart = 213;
        *gossipend = 223;
        StartStore(static_cast<char>(TalkID::GOSSIP));
      }
      break;
    case StoreOption::IDENTIFYANITEM:
      if (static_cast<TalkID>(*stextflag) == TalkID::STORY)
      {
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SIDENTIFY));
      }
      break;
    case StoreOption::EXIT:
      if (*stextflag) //need to refine so you cannot full exit from buy menu.
      {
        PlaySFX(70);
        *stextflag = static_cast<char>(TalkID::NONE);
      }
      break;
    case StoreOption::HEAL:
      if (static_cast<TalkID>(*stextflag) == TalkID::HEALER)
      {
        if (plr[*myplr]._pHitPoints != plr[*myplr]._pMaxHP)
          PlaySFX(82);
        *drawhpflag = TRUE;
        plr[*myplr]._pHitPoints = plr[*myplr]._pMaxHP;
        plr[*myplr]._pHPBase = plr[*myplr]._pMaxHPBase;
      }
      break;
    case StoreOption::BUYITEMS:
      switch (static_cast<TalkID>(*stextflag))
      {
      case TalkID::WITCH:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::WBUY));
        break;
      case TalkID::HEALER:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::HBUY));
        break;
      }
      break;
    case StoreOption::BUYBASIC:
      if (static_cast<TalkID>(*stextflag) == TalkID::SMITH)
      {
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SBUY));
      }
      break;
    case StoreOption::BUYPREMIUM:
      if (static_cast<TalkID>(*stextflag) == TalkID::SMITH)
      {
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SPBUY));
      }
      break;
    case StoreOption::SELL:
      switch (static_cast<TalkID>(*stextflag))
      {
      case TalkID::SMITH:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SSELL));
        break;
      case TalkID::WITCH:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::WSELL));
        break;
      }
      break;
    case StoreOption::REPAIR:
      if (static_cast<TalkID>(*stextflag) == TalkID::SMITH)
      {
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SREPAIR));
      }
      break;
    case StoreOption::RECHARGE:
      if (static_cast<TalkID>(*stextflag) == TalkID::WITCH)
      {
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::WRECHARGE));
      }
      break;
    case StoreOption::BACK:
      switch (static_cast<TalkID>(*stextflag))
      {
      case TalkID::SREPAIR:
      case TalkID::SSELL:
      case TalkID::SBUY:
      case TalkID::SPBUY:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::SMITH));
        break;
      case TalkID::WBUY:
      case TalkID::WSELL:
      case TalkID::WRECHARGE:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::WITCH));
        break;
      case TalkID::HBUY:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::HEALER));
        break;
      case TalkID::SIDENTIFY:
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::STORY));
        break;
      }
    }
  }

  void Server::buyItem(int itemID)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto witchitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A4EF8);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto stextshold = reinterpret_cast<int(*)>(0x69F288);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);
    auto SetCursor_ = reinterpret_cast<void(__fastcall*)(int)>(0x40746B);
    auto SpecialAutoPlace = reinterpret_cast<BOOL(__fastcall*)(int, int, int, int, BOOL)>(0x41C373);
    auto cursW = reinterpret_cast<int(*)>(0x4B8CA4);
    auto cursH = reinterpret_cast<int(*)>(0x4B8C9C);
    auto GetRndSeed = reinterpret_cast<int(__fastcall*)()>(0x41752C);
    auto TakePlrsMoney = reinterpret_cast<void(__fastcall*)(int)>(0x45A990);
    auto StoreAutoPlace = reinterpret_cast<void(__fastcall*)()>(0x458036);
    auto CalcPlrInv = reinterpret_cast<void(__fastcall*)(int, BOOL)>(0x41FD3E);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto smithitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A8A40);
    auto AutoPlace = reinterpret_cast<BOOL(__fastcall*)(int, int, int, int, BOOL)>(0x41C23F);
    auto premiumitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x69F290);
    auto numpremium = reinterpret_cast<int(*)>(0x6A6BBC);
    auto SpawnPremium = reinterpret_cast<void(__fastcall*)(int)>(0x4245A0);
    auto healitem = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A6BC0);
    auto gbMaxPlayers = reinterpret_cast<BYTE(*)>(0x679660);

    int idx, i;
    BOOL done, ok;

    if (static_cast<TalkID>(*stextflag) == TalkID::WBUY)
    {
      
      idx = -1;

      for (int i = 0; i < 20; i++)
      {
        if (data->itemList[itemID].compare(witchitem[i]))
        {
          idx = i;
          break;
        }
      }

      if (idx == -1)
        return;

      if (plr[*myplr]._pGold < witchitem[idx]._iIvalue)
      {
        *stextshold = *stextflag;
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::NOMONEY));
      }
      else
      {
        plr[*myplr].HoldItem = witchitem[idx];
        SetCursor_(plr[*myplr].HoldItem._iCurs + 12);
        done = FALSE;

        for (i = 0; i < 40 && !done; i++)
          done = SpecialAutoPlace(*myplr, i, *cursW / 28, *cursH / 28, FALSE);

        if (done)
        {
          if (idx < 3)
            plr[*myplr].HoldItem._iSeed = GetRndSeed();

          TakePlrsMoney(plr[*myplr].HoldItem._iIvalue);
          StoreAutoPlace();

          if (idx >= 3)
          {
            if (idx == 19)
              witchitem[19]._itype = -1;
            else
            {
              for (; witchitem[idx + 1]._itype != -1; idx++)
                witchitem[idx] = witchitem[idx + 1];

              witchitem[idx]._itype = -1;
            }
          }

          CalcPlrInv(*myplr, TRUE);
          PlaySFX(70);
          StartStore(*stextflag);
        }
        else
        {
          *stextshold = *stextflag;
          PlaySFX(70);
          StartStore(static_cast<char>(TalkID::NOROOM));
        }

        SetCursor_(1);
      }
    }
    else if (static_cast<TalkID>(*stextflag) == TalkID::SBUY)
    {
      idx = -1;

      for (int i = 0; i < 20; i++)
      {
        if (data->itemList[itemID].compare(smithitem[i]))
        {
          idx = i;
          break;
        }
      }

      if (idx == -1)
        return;

      if (plr[*myplr]._pGold < smithitem[idx]._iIvalue)
      {
        *stextshold = *stextflag;
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::NOMONEY));
      }
      else
      {
        plr[*myplr].HoldItem = smithitem[idx];
        SetCursor_(plr[*myplr].HoldItem._iCurs + 12);
        done = FALSE;

        for (i = 0; i < 40 && !done; i++)
          done = AutoPlace(*myplr, i, *cursW / 28, *cursH / 28, FALSE);
        if (done)
        {
          TakePlrsMoney(plr[*myplr].HoldItem._iIvalue);
          if (plr[*myplr].HoldItem._iMagical == 0)
            plr[*myplr].HoldItem._iIdentified = FALSE;
          StoreAutoPlace();

          if (idx == 19)
            smithitem[19]._itype = -1;
          else
          {
            for (; smithitem[idx + 1]._itype != -1; idx++)
              smithitem[idx] = smithitem[idx + 1];
            smithitem[idx]._itype = -1;
          }
          CalcPlrInv(*myplr, TRUE);
          PlaySFX(70);
          StartStore(*stextflag);
        }
        else
        {
          *stextshold = *stextflag;
          PlaySFX(70);
          StartStore(static_cast<char>(TalkID::NOROOM));
        }
        SetCursor_(1);
      }
    }
    else if (static_cast<TalkID>(*stextflag) == TalkID::SPBUY)
    {
      int idx = -1;
      for (int i = 0; i < 20; i++)
      {
        if (data->itemList[itemID].compare(premiumitem[i]))
        {
          idx = i;
          break;
        }
      }

      if (idx == -1)
        return;

      if (plr[*myplr]._pGold < premiumitem[idx]._iIvalue)
      {
        *stextshold = *stextflag;
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::NOMONEY));
      }
      else
      {
        plr[*myplr].HoldItem = premiumitem[idx];
        SetCursor_(plr[*myplr].HoldItem._iCurs + 12);
        done = FALSE;
        for (i = 0; i < 40 && !done; i++)
          done = AutoPlace(*myplr, i, *cursW / 28, *cursH / 28, FALSE);
        if (done)
        {
          TakePlrsMoney(plr[*myplr].HoldItem._iIvalue);
          if (plr[*myplr].HoldItem._iMagical == 0)
            plr[*myplr].HoldItem._iIdentified = FALSE;
          StoreAutoPlace();

          premiumitem[idx]._itype = -1;
          *numpremium -= 1;
          SpawnPremium(plr[*myplr]._pLevel);
          PlaySFX(70);
          StartStore(*stextflag);
        }
        else
        {
          *stextshold = *stextflag;
          PlaySFX(70);
          StartStore(static_cast<char>(TalkID::NOROOM));
        }
        SetCursor_(1);
      }
    }
    else if (static_cast<TalkID>(*stextflag) == TalkID::HBUY)
    {
      idx = -1;

      for (int i = 0; i < 20; i++)
      {
        if (data->itemList[itemID].compare(healitem[i]))
        {
          idx = i;
          break;
        }
      }

      if (idx == -1)
        return;

      if (plr[*myplr]._pGold < healitem[idx]._iIvalue)
      {
        *stextshold = *stextflag;
        PlaySFX(70);
        StartStore(static_cast<char>(TalkID::NOMONEY));
      }
      else
      {
        plr[*myplr].HoldItem = healitem[idx];
        SetCursor_(plr[*myplr].HoldItem._iCurs + 12);
        done = FALSE;
        i = 0;
        for (i = 0; i < 40 && !done; i++)
          done = SpecialAutoPlace(*myplr, i, *cursW / 28, *cursH / 28, FALSE);
        if (done)
        {
          ok = FALSE;
          if (*gbMaxPlayers == 1)
          {
            if (idx < 2)
              ok = TRUE;
          }
          else
          {
            if (idx < 3)
              ok = TRUE;
          }
          if (ok)
            plr[*myplr].HoldItem._iSeed = GetRndSeed();

          TakePlrsMoney(plr[*myplr].HoldItem._ivalue);
          if (plr[*myplr].HoldItem._iMagical == 0)
            plr[*myplr].HoldItem._iIdentified = FALSE;
          StoreAutoPlace();

          ok = FALSE;
          if (*gbMaxPlayers == 1)
          {
            if (idx >= 2)
              ok = TRUE;
          }
          else
          {
            if (idx >= 3)
              ok = TRUE;
          }
          if (ok)
          {
            if (idx == 19)
              healitem[19]._itype = -1;
            else
            {
              for (; healitem[idx + 1]._itype != -1; idx++)
                healitem[idx] = healitem[idx + 1];
              healitem[idx]._itype = -1;
            }
            CalcPlrInv(*myplr, TRUE);
            PlaySFX(70);
            StartStore(*stextflag);
          }
        }
        else
        {
          *stextshold = *stextflag;
          PlaySFX(70);
          StartStore(static_cast<char>(TalkID::NOROOM));
        }
        SetCursor_(1);
      }
    }
  }

  void Server::sellItem(int itemID)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto storehold = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A09F0);
    auto storehidx = reinterpret_cast<char(*)>(0x6A89F0);
    auto RemoveInvItem = reinterpret_cast<void(__fastcall*)(int, int)>(0x41D722);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto RemoveSpdBarItem = reinterpret_cast<void(__fastcall*)(int, int)>(0x41D810);
    auto storenumh = reinterpret_cast<int(*)>(0x69F10C);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto SetGoldCurs = reinterpret_cast<void(__fastcall*)(int, int)>(0x45A904);
    auto PlaceStoreGold = reinterpret_cast<void(__fastcall*)(int)>(0x45AF48);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);

    int i, idx, cost;

    if (static_cast<TalkID>(*stextflag) != TalkID::WSELL && static_cast<TalkID>(*stextflag) != TalkID::SSELL)
      return;

    idx = -1;

    for (int i = 0; i < 20; i++)
    {
      if (data->itemList[itemID].compare(storehold[i]))
      {
        idx = i;
        break;
      }
    }

    if (idx == -1)
      return;

    if (storehidx[idx] >= 0)
      RemoveInvItem(*myplr, storehidx[idx]);
    else
      RemoveSpdBarItem(*myplr, -(storehidx[idx] + 1));
    cost = storehold[idx]._iIvalue;
    *storenumh -= 1;
    if (idx != *storenumh)
    {
      while (idx < *storenumh)
      {
        storehold[idx] = storehold[idx + 1];
        storehidx[idx] = storehidx[idx + 1];
        idx++;
      }
    }
    plr[*myplr]._pGold += cost;
    for (i = 0; i < plr[*myplr]._pNumInv && cost > 0; i++)
    {
      if (plr[*myplr].InvList[i]._itype == 11 && plr[*myplr].InvList[i]._ivalue != 5000)
      {
        if (cost + plr[*myplr].InvList[i]._ivalue <= 5000)
        {
          plr[*myplr].InvList[i]._ivalue += cost;
          SetGoldCurs(*myplr, i);
          cost = 0;
        }
        else
        {
          cost -= 5000 - plr[*myplr].InvList[i]._ivalue;
          plr[*myplr].InvList[i]._ivalue = 5000;
          SetGoldCurs(*myplr, i);
        }
      }
    }
    if (cost > 0)
    {
      while (cost > 5000)
      {
        PlaceStoreGold(5000);
        cost -= 5000;
      }
      PlaceStoreGold(cost);
    }

    PlaySFX(70);
    StartStore(*stextflag);
  }

  void Server::rechargeItem(int itemID)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto storehold = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A09F0);
    auto TakePlrsMoney = reinterpret_cast<void(__fastcall*)(int)>(0x45A990);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto storehidx = reinterpret_cast<char(*)>(0x6A89F0);
    auto CalcPlrInv = reinterpret_cast<void(__fastcall*)(int, BOOL)>(0x41FD3E);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);

    int i, idx;

    if (static_cast<TalkID>(*stextflag) != TalkID::WRECHARGE)
      return;

    idx = -1;

    for (int i = 0; i < 20; i++)
    {
      if (data->itemList[itemID].compare(storehold[i]))
      {
        idx = i;
        break;
      }
    }

    if (idx == -1)
      return;

    TakePlrsMoney(plr[*myplr].HoldItem._iIvalue);

    i = storehidx[idx];
    if (i < 0)
      plr[*myplr].InvBody[4]._iCharges = plr[*myplr].InvBody[4]._iMaxCharges;
    else
      plr[*myplr].InvList[i]._iCharges = plr[*myplr].InvList[i]._iMaxCharges;

    CalcPlrInv(*myplr, TRUE);
    PlaySFX(70);
    StartStore(*stextflag);
  }

  void Server::repairItem(int itemID)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto storehold = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A09F0);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto stextshold = reinterpret_cast<int(*)>(0x69F288);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);
    auto TakePlrsMoney = reinterpret_cast<void(__fastcall*)(int)>(0x45A990);
    auto storehidx = reinterpret_cast<char(*)>(0x6A89F0);

    int i, idx;

    if (static_cast<TalkID>(*stextflag) != TalkID::SREPAIR)
      return;

    idx = -1;

    for (int i = 0; i < 20; i++)
    {
      if (data->itemList[itemID].compare(storehold[i]))
      {
        idx = i;
        break;
      }
    }

    if (idx == -1)
      return;

    plr[*myplr].HoldItem = storehold[idx];
    if (plr[*myplr]._pGold < storehold[idx]._iIvalue)
    {
      *stextshold = *stextflag;
      PlaySFX(70);
      StartStore(static_cast<char>(TalkID::NOMONEY));
    }
    else
    {
      TakePlrsMoney(plr[*myplr].HoldItem._iIvalue);
      storehold[idx]._iDurability = storehold[idx]._iMaxDur;

      i = storehidx[idx];
      if (i < 0)
      {
        if (i == -1)
          plr[*myplr].InvBody[0]._iDurability = plr[*myplr].InvBody[0]._iMaxDur;
        if (i == -2)
          plr[*myplr].InvBody[6]._iDurability = plr[*myplr].InvBody[6]._iMaxDur;
        if (i == -3)
          plr[*myplr].InvBody[4]._iDurability = plr[*myplr].InvBody[4]._iMaxDur;
        if (i == -4)
          plr[*myplr].InvBody[5]._iDurability = plr[*myplr].InvBody[5]._iMaxDur;
      }
      else
        plr[*myplr].InvList[i]._iDurability = plr[*myplr].InvList[i]._iMaxDur;
      
      PlaySFX(70);
      StartStore(*stextflag);
    }
  }

  void Server::attackMonster(int index)
  {
    auto monster = reinterpret_cast<DiabloInternal::MonsterStruct(*)>(0x64D330);
    auto CanTalkToMonster = reinterpret_cast<BOOL(__fastcall*)(int)>(0x43AC0C);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);

    if (index < 0 || 199 < index)
      return;

    if (!OKToAct())
      return;

    if (!isOnScreen(monster[index]._mx, monster[index]._my))
      return;

    if (CanTalkToMonster(index))
      NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ATTACKID), index);
    else if (plr[*myplr].InvBody[4]._itype == 3 || plr[*myplr].InvBody[5]._itype == 3)
      NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::RATTACKID), index);
    else
      NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ATTACKID), index);
  }

  void Server::attackXY(int x, int y)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto NetSendCmdLoc = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y)>(0x43C8C7);

    if (!isOnScreen(x, y))
      return;

    if (!OKToAct())
      return;

    if (plr[*myplr].InvBody[4]._itype == 3 || plr[*myplr].InvBody[5]._itype == 3)
      NetSendCmdLoc(1u, static_cast<unsigned char>(CommandType::RATTACKXY), x, y);
    else
      NetSendCmdLoc(1u, static_cast<unsigned char>(CommandType::SATTACKXY), x, y);
  }

  void Server::operateObject(int index)
  {
    auto object = reinterpret_cast<DiabloInternal::ObjectStruct(*)>(0x679C38);
    auto nobjects = reinterpret_cast<int(*)>(0x679A34);
    auto objectactive = reinterpret_cast<int(*)>(0x679838);
    auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);

    if (index < 0 || 126 < index)
      return;

    if (!isOnScreen(object[index]._ox, object[index]._oy))
      return;

    if (!OKToAct())
      return;

    bool found = false;
    for (int i = 0; i < *nobjects; i++)
    {
      if (objectactive[i] == index)
      {
        found = true;
        break;
      }
    }

    if (!found)
      return;

    NetSendCmdLocParam1(1u, static_cast<unsigned char>(CommandType::OPOBJXY), object[index]._ox, object[index]._oy, index);
  }

  void Server::useBeltItem(int slot)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto UseInvItem = reinterpret_cast<BOOL(__fastcall*)(int, int)>(0x41ED29);

    if (slot < 0 || 7 < slot)
      return;

    if (plr[*myplr].SpdList[slot]._itype == -1)
      return;

    int cii = slot + 47;
    UseInvItem(*myplr, cii);
  }

  void Server::toggleCharacterScreen()
  {
    auto chrflag = reinterpret_cast<BOOL(*)>(0x4B896C);

    if (!OKToAct())
      return;

    *chrflag = !*chrflag;
  }

  void Server::increaseStat(CommandType commandType)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto NetSendCmdParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned short wParam1)>(0x43C9AB);

    int maxValue = 0;

    if (plr[*myplr]._pStatPts == 0)
      return;

    switch (commandType)
    {
    case CommandType::ADDSTR:
      switch (static_cast<DiabloInternal::PlayerClasses>(plr[*myplr]._pClass))
      {
      case DiabloInternal::PlayerClasses::WARRIOR:
        maxValue = 250;
        break;
      case DiabloInternal::PlayerClasses::ROGUE:
        maxValue = 55;
        break;
      case DiabloInternal::PlayerClasses::SORCERER:
        maxValue = 45;
        break;
      }
      if (plr[*myplr]._pBaseStr < maxValue)
      {
        NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ADDSTR), 1);
        plr[*myplr]._pStatPts -= 1;
      }
      break;
    case CommandType::ADDMAG:
      switch (static_cast<DiabloInternal::PlayerClasses>(plr[*myplr]._pClass))
      {
      case DiabloInternal::PlayerClasses::WARRIOR:
        maxValue = 50;
        break;
      case DiabloInternal::PlayerClasses::ROGUE:
        maxValue = 70;
        break;
      case DiabloInternal::PlayerClasses::SORCERER:
        maxValue = 250;
        break;
      }
      if (plr[*myplr]._pBaseMag < maxValue)
      {
        NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ADDMAG), 1);
        plr[*myplr]._pStatPts -= 1;
      }
      break;
    case CommandType::ADDDEX:
      switch (static_cast<DiabloInternal::PlayerClasses>(plr[*myplr]._pClass))
      {
      case DiabloInternal::PlayerClasses::WARRIOR:
        maxValue = 60;
        break;
      case DiabloInternal::PlayerClasses::ROGUE:
        maxValue = 250;
        break;
      case DiabloInternal::PlayerClasses::SORCERER:
        maxValue = 85;
        break;
      }
      if (plr[*myplr]._pBaseDex < maxValue)
      {
        NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ADDDEX), 1);
        plr[*myplr]._pStatPts -= 1;
      }
      break;
    case CommandType::ADDVIT:
      switch (static_cast<DiabloInternal::PlayerClasses>(plr[*myplr]._pClass))
      {
      case DiabloInternal::PlayerClasses::WARRIOR:
        maxValue = 100;
        break;
      case DiabloInternal::PlayerClasses::ROGUE:
        maxValue = 80;
        break;
      case DiabloInternal::PlayerClasses::SORCERER:
        maxValue = 80;
        break;
      }
      if (plr[*myplr]._pBaseVit < maxValue)
      {
        NetSendCmdParam1(1u, static_cast<unsigned char>(CommandType::ADDVIT), 1);
        plr[*myplr]._pStatPts -= 1;
      }
    }
  }

  void Server::getItem(int itemID)
  {
    auto invflag = reinterpret_cast<BOOL(*)>(0x634CB8);
    auto item = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x635A28);
    auto itemactive = reinterpret_cast<int(*)>(0x634CC8);
    auto numitems = reinterpret_cast<int(*)>(0x6414DC);
    auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);

    if (!OKToAct())
      return;

    bool found = false;

    for (auto i = 0; i < data->groundItems.size(); i++)
    {
      if (data->groundItems[i] == itemID)
        found = true;
      if (found)
        break;
    }

    if (!found)
      return;

    auto itemData = data->itemList[itemID];
    
    if (!isOnScreen(itemData._ix, itemData._iy))
      return;

    int index = -1;
    for (int i = 0; i < *numitems; i++)
    {
      if (itemData.compare(item[itemactive[i]]))
        index = itemactive[i];

      if (index != -1)
        break;
    }

    if (index == -1)
      return;

    if (*invflag)
      NetSendCmdLocParam1(1u, static_cast<unsigned char>(CommandType::GOTOGETITEM), itemData._ix, itemData._iy, index);
    else
      NetSendCmdLocParam1(1u, static_cast<unsigned char>(CommandType::GOTOAGETITEM), itemData._ix, itemData._iy, index);
  }

  void Server::setSpell(int spellID, DiabloInternal::SpellTypeID spellType)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto force_redraw = reinterpret_cast<int(*)>(0x52571C);

    if (spellID == -1)
      return;

    switch (spellType)
    {
    case DiabloInternal::SpellTypeID::SKILL:
      if (!(plr[*myplr]._pAblSpells & (1 << (spellID - 1))))
        return;
      break;
    case DiabloInternal::SpellTypeID::SPELL:
      if (!(plr[*myplr]._pMemSpells & (1 << (spellID - 1))))
        return;
      break;
    case DiabloInternal::SpellTypeID::SCROLL:
      if (!(plr[*myplr]._pScrlSpells & (1 << (spellID - 1))))
        return;
      break;
    case DiabloInternal::SpellTypeID::CHARGES:
      if ((plr[*myplr].InvBody[4]._iSpell != spellID && plr[*myplr].InvBody[5]._iSpell != spellID) || (plr[*myplr].InvBody[4]._iCharges == 0 && plr[*myplr].InvBody[5]._iCharges == 0))
        return;
      break;
    case DiabloInternal::SpellTypeID::INVALID:
    default:
      return;
      break;
    }

    plr[*myplr]._pRSpell = spellID;
    plr[*myplr]._pRSplType = static_cast<char>(spellType);
    *force_redraw = 255;
  }

  void Server::castSpell(int index)
  {
    auto cursmx = reinterpret_cast<int(*)>(0x4B8CC4);
    auto monster = reinterpret_cast<DiabloInternal::MonsterStruct(*)>(0x64D330);
    auto cursmy = reinterpret_cast<int(*)>(0x4B8CC8);
    auto CheckPlrSpell = reinterpret_cast<void(__fastcall*)()>(0x44FF6F);

    if (!OKToAct())
      return;

    *cursmx = monster[index]._mx;
    *cursmy = monster[index]._my;

    CheckPlrSpell();
  }

  void Server::toggleInventory()
  {
    auto invflag = reinterpret_cast<BOOL(*)>(0x634CB8);

    if (!OKToAct())
      return;

    *invflag = !*invflag;
  }

  void Server::putInCursor(int itemID)
  {
    auto invflag = reinterpret_cast<BOOL(*)>(0x634CB8);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto InvRect = reinterpret_cast<DiabloInternal::InvXY(*)>(0x47AE60);
    auto CheckInvCut = reinterpret_cast<void(__fastcall*)(int, int, int)>(0x41D378);

    if (!*invflag)
      return;

    if (!OKToAct())
      return;

    if (data->itemList.size() <= itemID)
      return;

    auto item = data->itemList[itemID];
    int mx, my;

    mx = 0;
    my = 0;
    for (int i = 0; i < 55; i++)
    {
      if (i < 7)
      {
        if (item.compare(plr[*myplr].InvBody[i]) && plr[*myplr].InvBody[i]._itype != -1)
        {
          switch (static_cast<EquipSlot>(i))
          {
          case EquipSlot::HEAD:
            mx = InvRect[0].X + 1;
            my = InvRect[0].Y - 1;
            break;
          case EquipSlot::LEFTRING:
            mx = InvRect[4].X + 1;
            my = InvRect[4].Y - 1;
            break;
          case EquipSlot::RIGHTRING:
            mx = InvRect[5].X + 1;
            my = InvRect[5].Y - 1;
            break;
          case EquipSlot::AMULET:
            mx = InvRect[6].X + 1;
            my = InvRect[6].Y - 1;
            break;
          case EquipSlot::LEFTHAND:
            mx = InvRect[7].X + 1;
            my = InvRect[7].Y - 1;
            break;
          case EquipSlot::RIGHTHAND:
            mx = InvRect[13].X + 1;
            my = InvRect[13].Y - 1;
            break;
          case EquipSlot::BODY:
            mx = InvRect[19].X + 1;
            my = InvRect[19].Y - 1;
            break;
          }
          break;
        }
      }
      else if (i < 47)
      {
        if (item.compare(plr[*myplr].InvList[i - 7]) && plr[*myplr].InvList[i - 7]._itype != -1 && i - 7 < plr[*myplr]._pNumInv)
        {
          for (int rect_index = 0; rect_index < 40; rect_index++)
          {
            if (plr[*myplr].InvGrid[rect_index] == i - 6)
            {
              int index = rect_index + 25;
              mx = InvRect[index].X + 1;
              my = InvRect[index].Y - 1;
              break;
            }
          }
          break;
        }
      }
      else
      {
        if (item.compare(plr[*myplr].SpdList[i - 47]) && plr[*myplr].SpdList[i - 47]._itype != -1)
        {
          int index = 18 + i;
          mx = 210 + (i - 47) * 30;
          my = 370;
          break;
        }
      }
    }
    if (mx != 0 && my != 0)
      CheckInvCut(*myplr, mx, my);
  }

  void Server::putCursorItem(int location)
  {
    auto InvRect = reinterpret_cast<DiabloInternal::InvXY(*)>(0x47AE60);
    auto CheckInvPaste = reinterpret_cast<void(__fastcall*)(int, int, int)>(0x41C783);
    auto myplr = reinterpret_cast<int(*)>(0x686444);

    int mx, my;
    EquipSlot equipLocation = static_cast<EquipSlot>(location);

    if (!data->invflag)
      return;

    if (12 <= data->pcurs && equipLocation <= EquipSlot::BELT8)
    {
      mx = 0;
      my = 0;
      switch (equipLocation)
      {
      case EquipSlot::HEAD:
        mx = InvRect[0].X + 1;
        my = InvRect[0].Y - 1;
        break;
      case EquipSlot::LEFTRING:
        mx = InvRect[4].X + 2;
        my = InvRect[4].Y - 20;
        break;
      case EquipSlot::RIGHTRING:
        mx = InvRect[5].X + 2;
        my = InvRect[5].Y - 20;
        break;
      case EquipSlot::AMULET:
        mx = InvRect[6].X + 2;
        my = InvRect[6].Y - 20;
        break;
      case EquipSlot::LEFTHAND:
        mx = InvRect[7].X + 1;
        my = InvRect[7].Y - 1;
        break;
      case EquipSlot::RIGHTHAND:
        mx = InvRect[13].X + 1;
        my = InvRect[13].Y - 1;
        break;
      case EquipSlot::BODY:
        mx = InvRect[19].X + 1;
        my = InvRect[19].Y - 1;
        break;
      default:
        if (equipLocation < EquipSlot::BELT1)
        {
          int index = 18 + location;
          mx = InvRect[index].X + 2;
          my = InvRect[index].Y - 20;
        }
        else
        {
          int index = 18 + location;
          mx = 210 + (location - 47) * 30;
          my = 370;
        }
        break;
      }
      CheckInvPaste(*myplr, mx, my);
    }
  }
  
  void Server::dropCursorItem()
  {
    auto TryInvPut = reinterpret_cast<int(__cdecl*)()>(0x41E2F9);
    auto NetSendCmdPItem = reinterpret_cast<void(__fastcall*)(unsigned char, unsigned char, unsigned char, unsigned char)>(0x43CCF8);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto SetCursor = reinterpret_cast<void(__fastcall*)(int)>(0x40746B);
    
    if (!isOnScreen(plr[*myplr]._px, plr[*myplr]._py))
      return;

    if (12 <= data->pcurs)
    {
      if (!TryInvPut())
        return;
      NetSendCmdPItem(1u, static_cast<unsigned char>(CommandType::PUTITEM), plr[*myplr]._px, plr[*myplr]._py);
      SetCursor(1);
    }
  }

  void Server::useItem(int itemID)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto UseInvItem = reinterpret_cast<BOOL(__fastcall*)(int, int)>(0x41ED29);

    if (!OKToAct())
      return;

    if (data->itemList.size() <= itemID)
      return;

    auto itemData = data->itemList[itemID];
    for (int i = 0; i < 8; i++)
    {
      if (plr[*myplr].SpdList[i]._itype != -1 && itemData.compare(plr[*myplr].SpdList[i]))
      {
        useBeltItem(i);
        return;
      }
    }

    for (int i = 0; i < plr[*myplr]._pNumInv; i++)
    {
      if (plr[*myplr].InvList[i]._itype != -1 && itemData.compare(plr[*myplr].InvList[i]))
      {
        UseInvItem(*myplr, i + 7);
        return;
      }
    }
  }

  void Server::identifyStoreItem(int itemID)
  {
    auto stextflag = reinterpret_cast<char(*)>(0x6AA705);
    auto storehold = reinterpret_cast<DiabloInternal::ItemStruct(*)>(0x6A09F0);
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto TakePlrsMoney = reinterpret_cast<void(__fastcall*)(int)>(0x45A990);
    auto CalcPlrInv = reinterpret_cast<void(__fastcall*)(int, BOOL)>(0x41FD3E);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto StartStore = reinterpret_cast<void(__fastcall*)(char)>(0x45A2D0);

    int idx;

    if (static_cast<TalkID>(*stextflag) != TalkID::SIDENTIFY)
      return;

    idx = -1;

    for (int i = 0; i < 20; i++)
    {
      if (data->itemList[itemID].compare(storehold[i]))
      {
        idx = i;
        break;
      }
    }

    if (idx == -1)
      return;

    for (int i = 0; i < 7; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvBody[i]))
        plr[*myplr].InvBody[i]._iIdentified = TRUE;
    }
    for (int i = 0; i < plr[*myplr]._pNumInv; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvList[i]))
        plr[*myplr].InvList[i]._iIdentified = TRUE;
    }
    TakePlrsMoney(100);
    CalcPlrInv(*myplr, TRUE);
    PlaySFX(70);
    StartStore(static_cast<char>(TalkID::SIDENTIFY));
  }

  void Server::castSpell(int x, int y)
  {
    auto cursmx = reinterpret_cast<int(*)>(0x4B8CC4);
    auto cursmy = reinterpret_cast<int(*)>(0x4B8CC8);
    auto CheckPlrSpell = reinterpret_cast<void(__fastcall*)()>(0x44FF6F);

    if (!OKToAct())
      return;

    if (!isOnScreen(x, y))
      return;

    *cursmx = x;
    *cursmy = y;

    CheckPlrSpell();
  }

  void Server::cancelQText()
  {
    auto qtextflag = reinterpret_cast<BOOLEAN(*)>(0x646D00);
    auto stream_stop = reinterpret_cast<void(__fastcall*)()>(0x4158E2);

    if (!data->qtextflag)
      return;

    *qtextflag = FALSE;
    data->qtextflag = false;
    stream_stop();
  }

  void Server::setFPS(int newFPS)
  {
    FPS = newFPS;
  }

  void Server::disarmTrap(int index)
  {
    auto NetSendCmdLocParam1 = reinterpret_cast<void(__fastcall*)(unsigned char bHiPri, unsigned char bCmd, unsigned char x, unsigned char y, int wParam1)>(0x43C8F3);
    auto object = reinterpret_cast<DiabloInternal::ObjectStruct(*)>(0x679C38);

    if (static_cast<DiabloInternal::cursor_id>(data->pcurs) != DiabloInternal::cursor_id::CURSOR_DISARM)
      return;

    if (static_cast<DiabloInternal::PlayerClasses>(data->playerList[data->player]._pClass) != DiabloInternal::PlayerClasses::ROGUE)
      return;

    NetSendCmdLocParam1(TRUE, static_cast<unsigned char>(CommandType::DISARMXY), object[index]._ox, object[index]._oy, index);
  }

  void Server::skillRepair(int itemID)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto DoRepair = reinterpret_cast<void(__fastcall*)(int pnum, int cii)>(0x422C9C);
    
    if (static_cast<DiabloInternal::cursor_id>(data->pcurs) != DiabloInternal::cursor_id::CURSOR_REPAIR)
      return;

    if (static_cast<DiabloInternal::PlayerClasses>(data->playerList[data->player]._pClass) != DiabloInternal::PlayerClasses::WARRIOR)
      return;

    if (!data->invflag)
      return;

    for (int i = 0; i < 7; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvBody[i]))
      {
        DoRepair(*myplr, i);
        return;
      }
    }
    for (int i = 0; i < MAXINV; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvList[i]))
      {
        DoRepair(*myplr, i + 7);
        return;
      }
    }
  }

  void Server::skillRecharge(int itemID)
  {
    auto plr = reinterpret_cast<DiabloInternal::PlayerStruct(*)>(0x686448);
    auto myplr = reinterpret_cast<int(*)>(0x686444);
    auto DoRecharge = reinterpret_cast<void(__fastcall*)(int pnum, int cii)>(0x422D6C);

    if (static_cast<DiabloInternal::cursor_id>(data->pcurs) != DiabloInternal::cursor_id::CURSOR_RECHARGE)
      return;

    if (static_cast<DiabloInternal::PlayerClasses>(data->playerList[data->player]._pClass) != DiabloInternal::PlayerClasses::SORCERER)
      return;

    for (int i = 0; i < 7; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvBody[i]))
      {
        DoRecharge(*myplr, i);
        return;
      }
    }
    for (int i = 0; i < MAXINV; i++)
    {
      if (data->itemList[itemID].compare(plr[*myplr].InvList[i]))
      {
        DoRecharge(*myplr, i + 7);
        return;
      }
    }
  }

  void Server::toggleMenu()
  {
    auto qtextflag = reinterpret_cast<BOOLEAN(*)>(0x646D00);
    auto gamemenu_handle_previous = reinterpret_cast<void(__fastcall*)()>(0x418866);

    qtextflag = FALSE;
    gamemenu_handle_previous();
    return;
  }

  void Server::saveGame()
  {
    auto sgpCurrentMenu = reinterpret_cast<DiabloInternal::TMenuItem(**)>(0x634480);
    auto sgSingleMenu = reinterpret_cast<DiabloInternal::TMenuItem(*)>(0x48E1B8);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);

    if (*sgpCurrentMenu != sgSingleMenu)
      return;

    PlaySFX(69);
    sgSingleMenu[0].fnMenu(TRUE);
    return;
  }

  void Server::quit()
  {
    auto sgpCurrentMenu = reinterpret_cast<DiabloInternal::TMenuItem(**)>(0x634480);
    auto sgSingleMenu = reinterpret_cast<DiabloInternal::TMenuItem(*)>(0x48E1B8);
    auto PlaySFX = reinterpret_cast<void(__fastcall*)(int)>(0x415B59);
    auto sgMultiMenu = reinterpret_cast<DiabloInternal::TMenuItem(*)>(0x48E200);

    if (*sgpCurrentMenu == sgSingleMenu)
    {
      PlaySFX(69);
      sgSingleMenu[4].fnMenu(TRUE);
    }
    else if (*sgpCurrentMenu == sgMultiMenu)
    {
      PlaySFX(69);
      sgMultiMenu[3].fnMenu(TRUE);
    }
    return;
  }

  void Server::clearCursor()
  {
    auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
    auto NewCursor = reinterpret_cast<int(__fastcall*)(int)>(0x40748E);

    if (*pcurs == static_cast<int>(DiabloInternal::cursor_id::CURSOR_REPAIR) ||
      *pcurs == static_cast<int>(DiabloInternal::cursor_id::CURSOR_DISARM) ||
      *pcurs == static_cast<int>(DiabloInternal::cursor_id::CURSOR_RECHARGE))
      NewCursor(static_cast<int>(DiabloInternal::cursor_id::CURSOR_HAND));

    return;
  }
}