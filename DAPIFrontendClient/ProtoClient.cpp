#include "ProtoClient.h"
#include <chrono>
#include <thread>

namespace DAPI
{
  ProtoClient::ProtoClient()
  {}

  bool ProtoClient::isConnected() const
  {
    return protoClient.isConnected();
  }

  bool ProtoClient::connect()
  {
    if (protoClient.isConnected())
    {
      std::cout << "Already connected." << std::endl;
      return true;
    }

    while (true)
    {
      std::cout << "Connecting..." << std::endl;
      protoClient.lookForServer();
      if (protoClient.isConnected())
        return true;
      {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
      }
    }
    return protoClient.isConnected();
  }

  void ProtoClient::disconnect()
  {
    if (isConnected())
      return;
    protoClient.disconnect();
  }

  void ProtoClient::update(Game& game)
  {
    transmitMessages();
    hasCommand = false;
    if (protoClient.isConnected())
      protoClient.receiveMessages();

    if (protoClient.isConnected())
    {
      while (protoClient.messageQueueSize())
      {
        auto message = protoClient.getNextMessage();
        if (message->has_frameupdate())
        {
          auto frameUpdate = message->frameupdate();
          game.data->player = frameUpdate.player();
          game.data->stextflag = static_cast<char>(frameUpdate.stextflag());
          game.data->pauseMode = frameUpdate.pausemode();
          game.data->menuOpen = frameUpdate.menuopen();
          game.data->cursor = frameUpdate.cursor();
          game.data->chrflag = frameUpdate.chrflag();
          game.data->invflag = frameUpdate.invflag();
          game.data->qtextflag = frameUpdate.qtextflag();
          game.data->qtext = frameUpdate.qtext();
          game.data->currlevel = frameUpdate.currlevel();
          game.data->setlevel = frameUpdate.setlevel();
          game.data->FPS = frameUpdate.fps();
          for (int x = 0; x < 112; x++)
          {
            for (int y = 0; y < 112; y++)
            {
              game.data->dPiece[x][y].data->type = -1;
              game.data->dPiece[x][y].data->solid = false;
              game.data->dPiece[x][y].data->x = x;
              game.data->dPiece[x][y].data->y = y;
              game.data->dPiece[x][y].data->stopMissile = false;
            }
          }
          game.data->triggerList.clear();
          for (auto& trigger : frameUpdate.triggerdata())
          {
            game.data->triggerList.push_back(DAPI::Trigger{});
            game.data->triggerList[game.data->triggerList.size() - 1].data->lvl = trigger.lvl();
            game.data->triggerList[game.data->triggerList.size() - 1].data->x = trigger.x();
            game.data->triggerList[game.data->triggerList.size() - 1].data->y = trigger.y();
            game.data->triggerList[game.data->triggerList.size() - 1].data->type = static_cast<TriggerType>(trigger.type());
          }
          game.data->questList.clear();
          for (auto& quest : frameUpdate.questdata())
          {
            game.data->questList.push_back(DAPI::Quest{});
            game.data->questList[game.data->questList.size() - 1].data->id = static_cast<QuestID>(quest.id());
            game.data->questList[game.data->questList.size() - 1].data->state = static_cast<QuestState>(quest.state());
          }
          for (auto& dPiece : frameUpdate.dpiece())
          {
            game.data->dPiece[dPiece.x()][dPiece.y()].data->type = dPiece.type();
            game.data->dPiece[dPiece.x()][dPiece.y()].data->solid = dPiece.solid();
            game.data->dPiece[dPiece.x()][dPiece.y()].data->x = dPiece.x();
            game.data->dPiece[dPiece.x()][dPiece.y()].data->y = dPiece.y();
            game.data->dPiece[dPiece.x()][dPiece.y()].data->stopMissile = dPiece.stopmissile();
          }
          game.data->storeList.clear();
          for (auto& option : frameUpdate.storeoption())
            game.data->storeList.push_back(static_cast<StoreOption>(option));
          for (auto& townie : frameUpdate.townerdata())
          {
            auto townieID = townie.id();
            game.data->townerData[townieID].data->ID = townie.id();
            game.data->townerData[townieID].data->_ttype = static_cast<TalkerType>(townie._ttype());
            game.data->townerData[townieID].data->_tx = townie._tx();
            game.data->townerData[townieID].data->_ty = townie._ty();
            game.data->townerData[townieID].data->_tName = townie._tname();
          }
          for (auto& item : frameUpdate.itemdata())
          {
            auto itemID = item.id();
            game.data->itemList[itemID].data->ID = item.id();
            game.data->itemList[itemID].data->_itype = item._itype();
            game.data->itemList[itemID].data->_ix = item._ix();
            game.data->itemList[itemID].data->_iy = item._iy();
            game.data->itemList[itemID].data->_iIdentified = item._iidentified();
            game.data->itemList[itemID].data->_iMagical = item._imagical();
            game.data->itemList[itemID].data->_iName = item._iname();
            game.data->itemList[itemID].data->_iIName = item._iiname();
            game.data->itemList[itemID].data->_iClass = item._iclass();
            game.data->itemList[itemID].data->_iCurs = item._icurs();
            game.data->itemList[itemID].data->_ivalue = item._ivalue();
            game.data->itemList[itemID].data->_iMinDam = item._imindam();
            game.data->itemList[itemID].data->_iMaxDam = item._imaxdam();
            game.data->itemList[itemID].data->_iAC = item._iac();
            game.data->itemList[itemID].data->_iFlags = item._iflags();
            game.data->itemList[itemID].data->_iMiscId = item._imiscid();
            game.data->itemList[itemID].data->_iSpell = item._ispell();
            game.data->itemList[itemID].data->_iCharges = item._icharges();
            game.data->itemList[itemID].data->_iMaxCharges = item._imaxcharges();
            game.data->itemList[itemID].data->_iDurability = item._idurability();
            game.data->itemList[itemID].data->_iMaxDur = item._imaxdur();
            game.data->itemList[itemID].data->_iPLDam = item._ipldam();
            game.data->itemList[itemID].data->_iPLToHit = item._ipltohit();
            game.data->itemList[itemID].data->_iPLAC = item._iplac();
            game.data->itemList[itemID].data->_iPLStr = item._iplstr();
            game.data->itemList[itemID].data->_iPLMag = item._iplmag();
            game.data->itemList[itemID].data->_iPLDex = item._ipldex();
            game.data->itemList[itemID].data->_iPLVit = item._iplvit();
            game.data->itemList[itemID].data->_iPLFR = item._iplfr();
            game.data->itemList[itemID].data->_iPLLR = item._ipllr();
            game.data->itemList[itemID].data->_iPLMR = item._iplmr();
            game.data->itemList[itemID].data->_iPLMana = item._iplmana();
            game.data->itemList[itemID].data->_iPLHP = item._iplhp();
            game.data->itemList[itemID].data->_iPLDamMod = item._ipldammod();
            game.data->itemList[itemID].data->_iPLGetHit = item._iplgethit();
            game.data->itemList[itemID].data->_iPLLight = item._ipllight();
            game.data->itemList[itemID].data->_iSplLvlAdd = item._ispllvladd();
            game.data->itemList[itemID].data->_iFMinDam = item._ifmindam();
            game.data->itemList[itemID].data->_iFMaxDam = item._ifmaxdam();
            game.data->itemList[itemID].data->_iLMinDam = item._ilmindam();
            game.data->itemList[itemID].data->_iLMaxDam = item._ilmaxdam();
            game.data->itemList[itemID].data->_iPrePower = item._iprepower();
            game.data->itemList[itemID].data->_iSufPower = item._isufpower();
            game.data->itemList[itemID].data->_iMinStr = item._iminstr();
            game.data->itemList[itemID].data->_iMinMag = item._iminmag();
            game.data->itemList[itemID].data->_iMinDex = item._imindex();
            game.data->itemList[itemID].data->_iStatFlag = item._istatflag();
            game.data->itemList[itemID].data->IDidx = item.ididx();
          }
          for (auto& player : frameUpdate.playerdata())
          {
            int pnum = player.pnum();
            game.data->playerList[pnum].data->_pmode = static_cast<PlayerMode>(player._pmode());
            game.data->playerList[pnum].data->pnum = pnum;
            game.data->playerList[pnum].data->plrlevel = player.plrlevel();
            game.data->playerList[pnum].data->_px = player._px();
            game.data->playerList[pnum].data->_py = player._py();
            game.data->playerList[pnum].data->_pfutx = player._pfutx();
            game.data->playerList[pnum].data->_pfuty = player._pfuty();
            game.data->playerList[pnum].data->_pdir = player._pdir();

            game.data->playerList[pnum].data->_pRSpell = player._prspell();
            game.data->playerList[pnum].data->_pRSplType = player._prspltype();

            int i = 0;
            for (auto& SplLvl : player._pspllvl())
            {
              game.data->playerList[pnum].data->_pSplLvl[i] = SplLvl;
              i++;
            }
            game.data->playerList[pnum].data->_pMemSpells = player._pmemspells();
            game.data->playerList[pnum].data->_pAblSpells = player._pablspells();
            game.data->playerList[pnum].data->_pScrlSpells = player._pscrlspells();

            game.data->playerList[pnum].data->_pName = player._pname();
            game.data->playerList[pnum].data->_pClass = player._pclass();

            game.data->playerList[pnum].data->_pStrength = player._pstrength();
            game.data->playerList[pnum].data->_pBaseStr = player._pbasestr();
            game.data->playerList[pnum].data->_pMagic = player._pmagic();
            game.data->playerList[pnum].data->_pBaseMag = player._pbasemag();
            game.data->playerList[pnum].data->_pDexterity = player._pdexterity();
            game.data->playerList[pnum].data->_pBaseDex = player._pbasedex();
            game.data->playerList[pnum].data->_pVitality = player._pvitality();
            game.data->playerList[pnum].data->_pBaseVit = player._pbasevit();

            game.data->playerList[pnum].data->_pStatPts = player._pstatpts();

            game.data->playerList[pnum].data->_pDamageMod = player._pdamagemod();

            game.data->playerList[pnum].data->_pHitPoints = player._phitpoints();
            game.data->playerList[pnum].data->_pMaxHP = player._pmaxhp();
            game.data->playerList[pnum].data->_pMana = player._pmana();
            game.data->playerList[pnum].data->_pMaxMana = player._pmaxmana();
            game.data->playerList[pnum].data->_pLevel = player._plevel();
            game.data->playerList[pnum].data->_pExperience = player._pexperience();

            game.data->playerList[pnum].data->_pArmorClass = player._parmorclass();

            game.data->playerList[pnum].data->_pMagResist = player._pmagresist();
            game.data->playerList[pnum].data->_pFireResist = player._pfireresist();
            game.data->playerList[pnum].data->_pLightResist = player._plightresist();

            game.data->playerList[pnum].data->_pGold = player._pgold();

            game.data->playerList[pnum].data->InvBody.clear();
            int j = 0;
            for (auto& itemID : player.invbody())
            {
              if (itemID == -1)
                game.data->playerList[pnum].data->InvBody[j] = nullptr;
              else
                game.data->playerList[pnum].data->InvBody[j] = std::make_shared<Item>(game.data->itemList[itemID]);
              j++;
            }
            j = 0;
            for (auto& itemID : player.invlist())
            {
              if (itemID == -1)
                game.data->playerList[pnum].data->InvList[j] = nullptr;
              else
                game.data->playerList[pnum].data->InvList[j] = std::make_shared<Item>(game.data->itemList[itemID]);
              j++;
            }
            j = 0;
            for (auto& itemID : player.invgrid())
            {
              game.data->playerList[pnum].data->InvGrid[j] = itemID;
              j++;
            }
            j = 0;
            for (auto& itemID : player.spdlist())
            {
              if (itemID == -1)
                game.data->playerList[pnum].data->SpdList[j] = nullptr;
              else
                game.data->playerList[pnum].data->SpdList[j] = std::make_shared<Item>(game.data->itemList[itemID]);
              j++;
            }
            if (player.holditem() == -1)
              game.data->playerList[pnum].data->HoldItem = nullptr;
            else
              game.data->playerList[pnum].data->HoldItem = std::make_shared<Item>(game.data->itemList[player.holditem()]);

            game.data->playerList[pnum].data->_pIMinDam = player._pimindam();
            game.data->playerList[pnum].data->_pIMaxDam = player._pimaxdam();
            game.data->playerList[pnum].data->_pIBonusDam = player._pibonusdam();
            game.data->playerList[pnum].data->_pIAC = player._piac();
            game.data->playerList[pnum].data->_pIBonusToHit = player._pibonustohit();
            game.data->playerList[pnum].data->_pIBonusAC = player._pibonusac();
            game.data->playerList[pnum].data->_pIBonusDamMod = player._pibonusdammod();
          }
          game.data->groundItems.clear();
          for (auto& itemID : frameUpdate.grounditemid())
            game.data->groundItems.push_back(itemID);
          game.data->storeItems.clear();
          for (auto& itemID : frameUpdate.storeitems())
            game.data->storeItems.push_back(itemID);
          game.data->monsterList.clear();
          for (auto& monster : frameUpdate.monsterdata())
          {
            game.data->monsterList.push_back(Monster{});
            auto index = game.data->monsterList.size() - 1;
            game.data->monsterList[index].data->index = monster.index();
            game.data->monsterList[index].data->x = monster.x();
            game.data->monsterList[index].data->y = monster.y();
            game.data->monsterList[index].data->futx = monster.futx();
            game.data->monsterList[index].data->futy = monster.futy();
            game.data->monsterList[index].data->type = static_cast<MonsterType>(monster.type());
            game.data->monsterList[index].data->name = monster.name();
            game.data->monsterList[index].data->unique = monster.unique();
          }
          game.data->objectList.clear();
          for (auto& object : frameUpdate.objectdata())
          {
            game.data->objectList.push_back(Object{});
            auto index = game.data->objectList.size() - 1;
            game.data->objectList[index].data->type = static_cast<ObjectType>(object.type());
            game.data->objectList[index].data->x = object.x();
            game.data->objectList[index].data->y = object.y();
            game.data->objectList[index].data->shrineType = static_cast<ShrineType>(object.shrinetype());
            game.data->objectList[index].data->solid = object.solid();
            game.data->objectList[index].data->doorState = static_cast<DoorState>(object.doorstate());
            game.data->objectList[index].data->selectable = object.selectable();
            game.data->objectList[index].data->index = object.index();
            game.data->objectList[index].data->trapped = object.trapped();
          }
          game.data->missileList.clear();
          for (auto& missile : frameUpdate.missiledata())
          {
            game.data->missileList.push_back(Missile{});
            auto index = game.data->missileList.size() - 1;
            game.data->missileList[index].data->type = static_cast<MissileType>(missile.type());
            game.data->missileList[index].data->mixvel = missile.xvel();
            game.data->missileList[index].data->miyvel = missile.yvel();
            game.data->missileList[index].data->mix = missile.x();
            game.data->missileList[index].data->miy = missile.y();
            game.data->missileList[index].data->misx = missile.sx();
            game.data->missileList[index].data->misy = missile.sy();
          }
          game.data->portalList.clear();
          for (auto& portal : frameUpdate.portaldata())
          {
            game.data->portalList.push_back(Portal{});
            auto index = game.data->portalList.size() - 1;
            game.data->portalList[index].data->x = portal.x();
            game.data->portalList[index].data->y = portal.y();
            game.data->portalList[index].data->player = std::make_shared<Player>(game.data->playerList[portal.player()]);
          }
        }
        else if (message->has_endofqueue())
          break;
      }
    }
  }

  void ProtoClient::transmitMessages()
  {
    if (protoClient.isConnected())
      protoClient.transmitMessages();
  }

  bool ProtoClient::issueCommand(Command command)
  {
    if (this->isConnected() && !hasCommand)
    {
      auto newMessage = std::make_unique<dapi::message::Message>();
      auto commandMessage = newMessage->mutable_command();
      switch (command.type)
      {
      case CommandType::WALKXY:
      {
        auto move = commandMessage->mutable_move();
        move->set_targetx(command.targetX);
        move->set_targety(command.targetY);
      }
      break;
      case CommandType::TALKXY:
      {
        auto talk = commandMessage->mutable_talk();
        talk->set_targetx(command.targetX);
        talk->set_targety(command.targetY);
      }
      break;
      case CommandType::SELECTSTOREOPTION:
      {
        auto option = commandMessage->mutable_option();
        option->set_option(command.param1);
      }
      break;
      case CommandType::BUYITEM:
      {
        auto buyItem = commandMessage->mutable_buyitem();
        buyItem->set_id(command.param1);
      }
      break;
      case CommandType::SELLITEM:
      {
        auto sellItem = commandMessage->mutable_sellitem();
        sellItem->set_id(command.param1);
      }
      break;
      case CommandType::RECHARGEITEM:
      {
        auto rechargeItem = commandMessage->mutable_rechargeitem();
        rechargeItem->set_id(command.param1);
      }
      break;
      case CommandType::REPAIRITEM:
      {
        auto repairItem = commandMessage->mutable_repairitem();
        repairItem->set_id(command.param1);
      }
      break;
      case CommandType::ATTACKID:
      {
        auto attackMonster = commandMessage->mutable_attackmonster();
        attackMonster->set_index(command.param1);
      }
      break;
      case CommandType::ATTACKXY:
      {
        auto attackXY = commandMessage->mutable_attackxy();
        attackXY->set_x(command.targetX);
        attackXY->set_y(command.targetY);
      }
      break;
      case CommandType::OPERATEOBJ:
      case CommandType::OPENDOOR:
      case CommandType::CLOSEDOOR:
      {
        auto operateObject = commandMessage->mutable_operateobject();
        operateObject->set_index(command.param1);
      }
      break;
      case CommandType::NUM_CMDS:
      {
        auto useBeltItem = commandMessage->mutable_usebeltitem();
        useBeltItem->set_slot(command.param1);
      }
      break;
      case CommandType::CHARACTERSCREEN:
        commandMessage->mutable_togglecharactersheet();
        break;
      case CommandType::ADDSTR:
      case CommandType::ADDDEX:
      case CommandType::ADDMAG:
      case CommandType::ADDVIT:
      {
        auto increaseStat = commandMessage->mutable_increasestat();
        increaseStat->set_stat(static_cast<int>(command.type));
      }
      break;
      case CommandType::GETITEM:
      {
        auto getItem = commandMessage->mutable_getitem();
        getItem->set_id(static_cast<int>(command.param1));
      }
      break;
      case CommandType::SETSPELL:
      {
        auto setSpell = commandMessage->mutable_setspell();
        setSpell->set_spellid(command.param1);
        setSpell->set_spelltype(command.param2);
      }
      break;
      case CommandType::SPELLID:
      {
        auto castMonster = commandMessage->mutable_castmonster();
        castMonster->set_index(command.param1);
      }
      break;
      case CommandType::INVENTORYSCREEN:
      {
        auto inventoryScreen = commandMessage->mutable_toggleinventory();
      }
        break;
      case CommandType::PUTINCURSOR:
      {
        auto putInCursor = commandMessage->mutable_putincursor();
        putInCursor->set_id(command.param1);
      }
      break;
      case CommandType::PUTCURSORITEM:
      {
        auto putCursorItem = commandMessage->mutable_putcursoritem();
        putCursorItem->set_target(command.param1);
      }
      break;
      case CommandType::DROPITEM:
      {
        auto dropItem = commandMessage->mutable_dropcursoritem();
      }
      break;
      case CommandType::USEITEM:
      {
        auto useItem = commandMessage->mutable_useitem();
        useItem->set_id(command.param1);
      }
      break;
      case CommandType::IDENTIFYSTOREITEM:
      {
        auto identifyStoreItem = commandMessage->mutable_identifystoreitem();
        identifyStoreItem->set_id(command.param1);
      }
      break;
      case CommandType::SPELLXY:
      {
        auto castXY = commandMessage->mutable_castxy();
        castXY->set_x(command.param1);
        castXY->set_y(command.param2);
      }
      break;
      case CommandType::CANCELQTEXT:
      {
        auto cancelQText = commandMessage->mutable_cancelqtext();
      }
      break;
      case CommandType::SETFPS:
      {
        auto setFPS = commandMessage->mutable_setfps();
        setFPS->set_fps(command.param1);
      }
      break;
      case CommandType::DISARMXY:
      {
        auto disarmTrap = commandMessage->mutable_disarmtrap();
        disarmTrap->set_index(command.param1);
      }
      break;
      case CommandType::SKILLREPAIR:
      {
        auto skillRepair = commandMessage->mutable_skillrepair();
        skillRepair->set_id(command.param1);
      }
      break;
      case CommandType::SKILLRECHARGE:
      {
        auto skillRecharge = commandMessage->mutable_skillrecharge();
        skillRecharge->set_id(command.param1);
      }
      break;
      default:
        break;
      }
      protoClient.queueMessage(std::move(newMessage));
      hasCommand = true;
      if (command.type == CommandType::SETFPS)
        hasCommand = false;
      return true;
    }
    return false;
  }
}