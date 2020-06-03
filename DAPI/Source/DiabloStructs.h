#pragma once


#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>

namespace DiabloInternal
{

  #define MAXPATHLEN		25

  #define PLR_NAME_LEN	32

  #define LVLLENGTH	4

  #define NUMLEVELS	((4*LVLLENGTH)+1)

  #define NUM_INVLOC		7
  #define MAXINV			40
  #define MAXSPD			8

  #define MAXITEMS 127

  #define MAXQUESTS 16

  typedef struct PortalStruct {
    BOOL open;
    int x;
    int y;
    int level;
    int ltype;
    BOOL setlvl;
  } PortalStruct;

  typedef struct InvXY {
    int X;
    int Y;
  } InvXY;

  typedef struct MissileStruct {
    int _mitype;
    int _mix;
    int _miy;
    int _mixoff;
    int _miyoff;
    int _mixvel;
    int _miyvel;
    int _misx;
    int _misy;
    int _mitxoff;
    int _mityoff;
    int _mimfnum;
    int _mispllvl;
    BOOL _miDelFlag;
    BYTE _miAnimType;
    int _miAnimFlags;
    unsigned char* _miAnimData;
    int _miAnimDelay;
    int _miAnimLen;
    int _miAnimWidth;
    int _miAnimWidth2;
    int _miAnimCnt;
    int _miAnimAdd;
    int _miAnimFrame;
    BOOL _miDrawFlag;
    BOOL _miLightFlag;
    BOOL _miPreFlag;
    int _miUniqTrans;
    int _mirange;
    int _misource;
    int _micaster;
    int _midam;
    BOOL _miHitFlag;
    int _midist;
    int _mlid;
    int _mirnd;
    int _miVar1;
    int _miVar2;
    int _miVar3;
    int _miVar4;
    int _miVar5;
    int _miVar6;
    int _miVar7;
    int _miVar8;
  } MissileStruct;

  enum struct SpellTypeID
  {
    SKILL = 0x0,
    SPELL = 0x1,
    SCROLL = 0x2,
    CHARGES = 0x3,
    INVALID = 4,
  };

  enum struct PlayerClasses {
    WARRIOR = 0x0,
    ROGUE = 0x1,
    SORCERER = 0x2,
    NUM_CLASSES,
  };

enum struct ObjectID {
  L1LIGHT = 0x0,
  L1LDOOR = 0x1,
  L1RDOOR = 0x2,
  SKFIRE = 0x3,
  LEVER = 0x4,
  CHEST1 = 0x5,
  CHEST2 = 0x6,
  CHEST3 = 0x7,
  CANDLE1 = 0x8,
  CANDLE2 = 0x9,
  CANDLEO = 0xA,
  BANNERL = 0xB,
  BANNERM = 0xC,
  BANNERR = 0xD,
  SKPILE = 0xE,
  SKSTICK1 = 0xF,
  SKSTICK2 = 0x10,
  SKSTICK3 = 0x11,
  SKSTICK4 = 0x12,
  SKSTICK5 = 0x13,
  CRUX1 = 0x14,
  CRUX2 = 0x15,
  CRUX3 = 0x16,
  STAND = 0x17,
  ANGEL = 0x18,
  BOOK2L = 0x19,
  BCROSS = 0x1A,
  NUDEW2R = 0x1B,
  SWITCHSKL = 0x1C,
  TNUDEM1 = 0x1D,
  TNUDEM2 = 0x1E,
  TNUDEM3 = 0x1F,
  TNUDEM4 = 0x20,
  TNUDEW1 = 0x21,
  TNUDEW2 = 0x22,
  TNUDEW3 = 0x23,
  TORTURE1 = 0x24,
  TORTURE2 = 0x25,
  TORTURE3 = 0x26,
  TORTURE4 = 0x27,
  TORTURE5 = 0x28,
  BOOK2R = 0x29,
  L2LDOOR = 0x2A,
  L2RDOOR = 0x2B,
  TORCHL = 0x2C,
  TORCHR = 0x2D,
  TORCHL2 = 0x2E,
  TORCHR2 = 0x2F,
  SARC = 0x30,
  FLAMEHOLE = 0x31,
  FLAMELVR = 0x32,
  WATER = 0x33,
  BOOKLVR = 0x34,
  TRAPL = 0x35,
  TRAPR = 0x36,
  BOOKSHELF = 0x37,
  WEAPRACK = 0x38,
  BARREL = 0x39,
  BARRELEX = 0x3A,
  SHRINEL = 0x3B,
  SHRINER = 0x3C,
  SKELBOOK = 0x3D,
  BOOKCASEL = 0x3E,
  BOOKCASER = 0x3F,
  BOOKSTAND = 0x40,
  BOOKCANDLE = 0x41,
  BLOODFTN = 0x42,
  DECAP = 0x43,
  TCHEST1 = 0x44,
  TCHEST2 = 0x45,
  TCHEST3 = 0x46,
  BLINDBOOK = 0x47,
  BLOODBOOK = 0x48,
  PEDISTAL = 0x49,
  L3LDOOR = 0x4A,
  L3RDOOR = 0x4B,
  PURIFYINGFTN = 0x4C,
  ARMORSTAND = 0x4D,
  ARMORSTANDN = 0x4E,
  GOATSHRINE = 0x4F,
  CAULDRON = 0x50,
  MURKYFTN = 0x51,
  TEARFTN = 0x52,
  ALTBOY = 0x53,
  MCIRCLE1 = 0x54,
  MCIRCLE2 = 0x55,
  STORYBOOK = 0x56,
  STORYCANDLE = 0x57,
  STEELTOME = 0x58,
  WARARMOR = 0x59,
  WARWEAP = 0x5A,
  TBCROSS = 0x5B,
  WEAPONRACK = 0x5C,
  WEAPONRACKN = 0x5D,
  MUSHPATCH = 0x5E,
  LAZSTAND = 0x5F,
  SLAINHERO = 0x60,
  SIGNCHEST = 0x61,
  NULL_98 = 0x62,
};

  typedef struct ObjectStruct {
    int _otype;
    int _ox;
    int _oy;
    int _oLight;
    int _oAnimFlag;
    unsigned char* _oAnimData;
    int _oAnimDelay;
    int _oAnimCnt;
    int _oAnimLen;
    int _oAnimFrame;
    int _oAnimWidth;
    int _oAnimWidth2;
    BOOL _oDelFlag;
    char _oBreak; // check
    BOOL _oSolidFlag;
    BOOL _oMissFlag;
    char _oSelFlag; // check
    BOOL _oPreFlag;
    BOOL _oTrapFlag;
    BOOL _oDoorFlag;
    int _olid;
    int _oRndSeed;
    int _oVar1;
    int _oVar2;
    int _oVar3;
    int _oVar4;
    int _oVar5;
    int _oVar6;
    int _oVar7;
    int _oVar8;
  } ObjectStruct;

  typedef struct CKINFO {
    DWORD dwSize;
    DWORD dwOffset;
  } CKINFO;

  typedef struct TSnd {
    WAVEFORMATEX fmt;
    CKINFO chunk;
    char* sound_path;
    LPDIRECTSOUNDBUFFER DSB;
    int start_tc;
  } TSnd;

  typedef struct AnimStruct {
    BYTE* CMem;
    BYTE* Data[8];
    int Frames;
    int Rate;
  } AnimStruct;

  typedef struct MonsterData {
    int width;
    int mImage;
    char* GraphicType;
    BOOL has_special;
    char* sndfile;
    BOOL snd_special;
    BOOL has_trans;
    char* TransFile;
    int Frames[6];
    int Rate[6];
    char* mName;
    char mMinDLvl;
    char mMaxDLvl;
    char mLevel;
    int mMinHP;
    int mMaxHP;
    char mAi;
    int mFlags;
    unsigned char mInt;
    unsigned char mHit;
    unsigned char mAFNum;
    unsigned char mMinDamage;
    unsigned char mMaxDamage;
    unsigned char mHit2;
    unsigned char mAFNum2;
    unsigned char mMinDamage2;
    unsigned char mMaxDamage2;
    unsigned char mArmorClass;
    char mMonstClass;
    unsigned short mMagicRes;
    unsigned short mMagicRes2;
    unsigned short mTreasure;
    char mSelFlag;
    unsigned short mExp;
  } MonsterData;

  typedef struct CMonster {
    unsigned char mtype;
    unsigned char mPlaceFlags;
    AnimStruct Anims[6];
    TSnd* Snds[4][2];
    int width;
    int width2;
    unsigned char mMinHP;
    unsigned char mMaxHP;
    BOOL has_special;
    unsigned char mAFNum;
    char mdeadval;
    MonsterData* MData;
    BYTE* trans_file;
  } CMonster;

  typedef struct MonsterStruct {
    int _mMTidx;
    int _mmode;
    unsigned char _mgoal;
    int _mgoalvar1;
    int _mgoalvar2;
    int _mgoalvar3;
    int field_18;
    unsigned char _pathcount;
    int _mx;
    int _my;
    int _mfutx;
    int _mfuty;
    int _moldx;
    int _moldy;
    int _mxoff;
    int _myoff;
    int _mxvel;
    int _myvel;
    int _mdir;
    int _menemy;
    unsigned char _menemyx;
    unsigned char _menemyy;
    short falign_52;
    unsigned char* _mAnimData;
    int _mAnimDelay;
    int _mAnimCnt;
    int _mAnimLen;
    int _mAnimFrame;
    BOOL _meflag;
    BOOL _mDelFlag;
    int _mVar1;
    int _mVar2;
    int _mVar3;
    int _mVar4;
    int _mVar5;
    int _mVar6;
    int _mVar7;
    int _mVar8;
    int _mmaxhp;
    int _mhitpoints;
    unsigned char _mAi;
    unsigned char _mint;
    short falign_9A;
    int _mFlags;
    BYTE _msquelch;
    int falign_A4;
    int _lastx;
    int _lasty;
    int _mRndSeed;
    int _mAISeed;
    int falign_B8;
    unsigned char _uniqtype;
    unsigned char _uniqtrans;
    char _udeadval;
    char mWhoHit;
    char mLevel;
    unsigned short mExp;
    unsigned char mHit;
    unsigned char mMinDamage;
    unsigned char mMaxDamage;
    unsigned char mHit2;
    unsigned char mMinDamage2;
    unsigned char mMaxDamage2;
    unsigned char mArmorClass;
    char falign_CB;
    unsigned short mMagicRes;
    int mtalkmsg;
    unsigned char leader;
    unsigned char leaderflag;
    unsigned char packsize;
    unsigned char mlid;
    char* mName;
    CMonster* MType;
    MonsterData* MData;
  } MonsterStruct;

  typedef struct UniqMonstStruct {
    char mtype;
    char* mName;
    char* mTrnName;
    unsigned char mlevel;
    unsigned short mmaxhp;
    unsigned char mAi;
    unsigned char mint;
    unsigned char mMinDamage;
    unsigned char mMaxDamage;
    unsigned short mMagicRes;
    unsigned short mUnqAttr;
    unsigned char mUnqVar1;
    unsigned char mUnqVar2;
    int mtalkmsg;
  } UniqMonstStruct;

  typedef struct TriggerStruct {
    int _tx;
    int _ty;
    int _tmsg;
    int _tlvl;
  } TriggerStruct;

  typedef struct ScrollStruct {
    int _sxoff;
    int _syoff;
    int _sdx;
    int _sdy;
    int _sdir;
  } ScrollStruct;

  typedef struct STextStruct {
    int _sx;
    int _syoff;
    char _sstr[128];
    BOOL _sjust;
    char _sclr;
    int _sline;
    BOOL _ssel;
    int _sval;
  } STextStruct;

  typedef struct TNQ {
    unsigned char _qsttype;
    unsigned char _qstmsg;
    BOOLEAN _qstmsgact;
  } TNQ;

  typedef struct TownerStruct {
    int _tmode;
    int _ttype;
    int _tx;
    int _ty;
    int _txoff;
    int _tyoff;
    int _txvel;
    int _tyvel;
    int _tdir;
    unsigned char* _tAnimData;
    int _tAnimDelay;
    int _tAnimCnt;
    int _tAnimLen;
    int _tAnimFrame;
    int _tAnimFrameCnt;
    char _tAnimOrder;
    int _tAnimWidth;
    int _tAnimWidth2;
    int _tTenPer;
    int _teflag;
    int _tbtcnt;
    int _tSelFlag;
    BOOL _tMsgSaid;
    TNQ qsts[MAXQUESTS];
    int _tSeed;
    int _tVar1;
    int _tVar2;
    int _tVar3;
    int _tVar4;
    char _tName[PLR_NAME_LEN];
    unsigned char* _tNAnim[8];
    int _tNFrames;
    unsigned char* _tNData;
  } TownerStruct;

  typedef struct ItemStruct {
    int _iSeed;
    WORD _iCreateInfo;
    int _itype;
    int _ix;
    int _iy;
    BOOL _iAnimFlag;
    unsigned char* _iAnimData; // PSX name -> ItemFrame
    int _iAnimLen;
    int _iAnimFrame;
    int _iAnimWidth;
    int _iAnimWidth2; // width 2?

    BOOL _iDelFlag;   // set when item is flagged for deletion, deprecated in 1.02

    char _iSelFlag;
    BOOL _iPostDraw;

    BOOL _iIdentified;
    char _iMagical;
    char _iName[64];
    char _iIName[64];
    char _iLoc;
    char _iClass;
    int _iCurs;
    int _ivalue;
    int _iIvalue;
    int _iMinDam;
    int _iMaxDam;
    int _iAC;
    int _iFlags;
    int _iMiscId;
    int _iSpell;

    int _iCharges;
    int _iMaxCharges;

    int _iDurability;
    int _iMaxDur;

    int _iPLDam;
    int _iPLToHit;
    int _iPLAC;
    int _iPLStr;
    int _iPLMag;
    int _iPLDex;
    int _iPLVit;
    int _iPLFR;
    int _iPLLR;
    int _iPLMR;
    int _iPLMana;
    int _iPLHP;
    int _iPLDamMod;
    int _iPLGetHit;
    int _iPLLight;
    char _iSplLvlAdd;

    char _iRequest;
    int _iUid;

    int _iFMinDam;
    int _iFMaxDam;
    int _iLMinDam;
    int _iLMaxDam;

    int _iPLEnAc;

    char _iPrePower;
    char _iSufPower;

    int _iVAdd1;
    int _iVMult1;
    int _iVAdd2;
    int _iVMult2;

    char _iMinStr;
    unsigned char _iMinMag;
    char _iMinDex;
    BOOL _iStatFlag;
    int IDidx;
    int offs016C; // _oldlight or _iInvalid
  } ItemStruct;

  typedef struct PlayerStruct {
    int _pmode;
    char walkpath[MAXPATHLEN];
    BOOLEAN plractive;

    int destAction;
    int destParam1;
    int destParam2;
    int destParam3;
    int destParam4;
    int plrlevel;

    int _px;
    int _py;
    int _pfutx;
    int _pfuty;
    int _ptargx;
    int _ptargy;
    int _pownerx;
    int _pownery;
    int _poldx;
    int _poldy;

    int _pxoff;
    int _pyoff;
    int _pxvel;
    int _pyvel;
    int _pdir;
    int _nextdir;
    int _pgfxnum;
    unsigned char* _pAnimData;
    int _pAnimDelay;
    int _pAnimCnt;
    int _pAnimLen;
    int _pAnimFrame;
    int _pAnimWidth;
    int _pAnimWidth2;
    int _peflag;

    int _plid;
    int _pvid;

    int _pSpell;
    char _pSplType;
    char _pSplFrom;

    int _pTSpell;
    char _pTSplType;

    int _pRSpell;
    char _pRSplType;

    int _pSBkSpell;
    char _pSBkSplType;

    char _pSplLvl[64];
    unsigned __int64 _pMemSpells;
    unsigned __int64 _pAblSpells;
    unsigned __int64 _pScrlSpells;
    UCHAR _pSpellFlags;

    int _pSplHotKey[4];
    char _pSplTHotKey[4];

    int _pwtype;

    BOOLEAN _pBlockFlag;
    BOOLEAN _pInvincible;
    char _pLightRad;
    BOOLEAN _pLvlChanging;

    char _pName[PLR_NAME_LEN];
    char _pClass;

    int _pStrength;
    int _pBaseStr;
    int _pMagic;
    int _pBaseMag;
    int _pDexterity;
    int _pBaseDex;
    int _pVitality;
    int _pBaseVit;

    int _pStatPts;

    int _pDamageMod;
    int _pBaseToBlk;

    int _pHPBase;
    int _pMaxHPBase;
    int _pHitPoints;
    int _pMaxHP;
    int _pHPPer;
    int _pManaBase;
    int _pMaxManaBase;
    int _pMana;
    int _pMaxMana;
    int _pManaPer;

    char _pLevel;
    char _pMaxLvl;
    int _pExperience;
    int _pMaxExp;
    int _pNextExper;

    char _pArmorClass;

    char _pMagResist;
    char _pFireResist;
    char _pLghtResist;

    int _pGold;

    BOOL _pInfraFlag;

    int _pVar1;
    int _pVar2;
    int _pVar3;
    int _pVar4;
    int _pVar5;
    int _pVar6;
    int _pVar7;
    int _pVar8;

    BOOLEAN _pLvlVisited[NUMLEVELS];
    BOOLEAN _pSLvlVisited[NUMLEVELS]; // only 10 used

    int _pGFXLoad;

    unsigned char* _pNAnim[8];
    int _pNFrames;
    int _pNWidth;
    unsigned char* _pWAnim[8];
    int _pWFrames;
    int _pWWidth;
    unsigned char* _pAAnim[8];
    int _pAFrames;
    int _pAWidth;
    int _pAFNum;
    unsigned char* _pLAnim[8];
    unsigned char* _pFAnim[8];
    unsigned char* _pTAnim[8];
    int _pSFrames;
    int _pSWidth;
    int _pSFNum;
    unsigned char* _pHAnim[8];
    int _pHFrames;
    int _pHWidth;
    unsigned char* _pDAnim[8];
    int _pDFrames;
    int _pDWidth;
    unsigned char* _pBAnim[8];
    int _pBFrames;
    int _pBWidth;

    ItemStruct InvBody[NUM_INVLOC];
    ItemStruct InvList[MAXINV];
    int _pNumInv;
    char InvGrid[MAXINV];
    ItemStruct SpdList[MAXSPD];
    ItemStruct HoldItem;

    int _pIMinDam;
    int _pIMaxDam;
    int _pIAC;
    int _pIBonusDam;
    int _pIBonusToHit;
    int _pIBonusAC;
    int _pIBonusDamMod;
    unsigned __int64 _pISpells;
    int _pIFlags;
    int _pIGetHit;
    char _pISplLvlAdd;
    char _pISplCost;
    int _pISplDur;
    int _pIEnAc;
    int _pIFMinDam;
    int _pIFMaxDam;
    int _pILMinDam;
    int _pILMaxDam;

    int _pOilType;

    unsigned char pTownWarps;
    unsigned char pDungMsgs;
    unsigned char pLvlLoad;
    unsigned char pBattleNet;
    BOOLEAN pManaShield;
    char bReserved[3];
    short wReserved[8];

    DWORD pDiabloKillLevel;
    int dwReserved[7];
    unsigned char* _pNData;
    unsigned char* _pWData;
    unsigned char* _pAData;
    unsigned char* _pLData;
    unsigned char* _pFData;
    unsigned char* _pTData;
    unsigned char* _pHData;
    unsigned char* _pDData;
    unsigned char* _pBData;
    void* pReserved;
  } PlayerStruct;
}