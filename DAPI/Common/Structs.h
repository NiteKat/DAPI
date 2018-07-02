#pragma once
#include<dsound.h>

struct ItemStruct
{
	int _iSeed;
	unsigned short _iCreateInfo;
	int _itype;
	int _ix;
	int _iy;
	int _iAnimFlag;
	int _iAnimData; // unsigned char *
	int _iAnimLen;
	int _iAnimFrame;
	int _iAnimWidth;
	int _iAnimWidth2; // width 2?
	int offs002C;
	char _iSelFlag;
	int _iPostDraw;
	int _iIdentified;
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
	int _iStatFlag;
	int IDidx;
	int offs016C;
};

struct MonsterData
{
	int flags; // width?
	int mType;
	char *GraphicType;
	int has_special;
	char *sndfile;
	int snd_special;
	int has_trans;
	char *TransFile;
	int Frames[6];
	int Rate[6];
	char *mName;
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
	char mArmorClass;
	char mMonstClass;
	unsigned short mMagicRes;
	unsigned short mMagicRes2;
	unsigned short mTreasure;
	char mSelFlag;
	unsigned short mExp;
};

struct AnimStruct
{
	int CMem; // [unsigned] char * ??
	int Frames[8]; // unsigned char *
	int Rate;
	int Delay;
};

struct TSnd
{
	WAVEFORMATEX fmt;
	int len;
	int offset;
	char *sound_path;
	IDirectSoundBuffer *DSB;
	int start_tc;
};

struct CMonster
{
	unsigned char mtype;
	unsigned char mPlaceFlags;
	AnimStruct Anims[6];
	TSnd *Snds[8];
	int flags_1; // width
	int flags_2; // width 2
	unsigned char mMinHP;
	unsigned char mMaxHP;
	int has_special;
	unsigned char mAFNum;
	char mdeadval;
	MonsterData *MData;
	void *trans_file;
};

struct MonsterStruct
{
	int _mMTidx;
	int _mmode;
	int _mgoal;
	int _mgoalvar1;
	int _mgoalvar2;
	int _mgoalvar3;
	int field_18;
	int _pathcount;
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
	int _mAFNum;
	int _mAnimDelay;
	int _mAnimCnt;
	int _mAnimLen;
	int _mAnimFrame;
	int _meflag;
	int _mDelFlag;
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
	int _msquelch;
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
	short mLevel; /* char */
	unsigned short mExp;
	unsigned char mHit;
	unsigned char mMinDamage;
	unsigned char mMaxDamage;
	unsigned char mHit2;
	unsigned char mMinDamage2;
	unsigned char mMaxDamage2;
	char mArmorClass;
	char falign_CB;
	int mMagicRes; /* ushort */
	int mtalkmsg;
	unsigned char leader;
	unsigned char leaderflag;
	unsigned char unpackfilesize;
	unsigned char mlid;
	char *mName;
	CMonster *MType;
	MonsterData *MData;
};

struct UniqMonstStruct
{
	char mtype;
	char *mName;
	char *mMode;
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
};

struct PlayerStruct
{
	int _pmode;
	char walkpath[25];
	unsigned char plractive;
	int destAction;
	int destParam1;
	int destParam2;
	int destParam3;
	int destParam4;
	int plrlevel;
	int WorldX;
	int WorldY;
	int _px;
	int _py;
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
	int _pAnimData; // unsigned char *
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
	int _pTSplType;
	int _pRSpell;
	int _pRSplType;
	int _pSBkSpell;
	char _pSBkSplType;
	char _pSplLvl[64];
	int remove_1;
	int _pMemSpells[2]; // __declspec(align(8))
	int _pAblSpells[2];
	int _pScrlSpells[2];
	int _pSpellFlags;
	int _pSplHotKey[4];
	char _pSplTHotKey[4];
	int _pwtype;
	unsigned char _pBlockFlag;
	unsigned char _pInvincible;
	char _pLightRad;
	unsigned char _pLvlChanging;
	char _pName[32];
	int _pClass;
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
	int _pInfraFlag;
	int _pVar1;
	int _pVar2;
	int _pVar3;
	int _pVar4;
	int _pVar5;
	int _pVar6;
	int _pVar7;
	int _pVar8;
	unsigned char _pLvlVisited[17];
	unsigned char _pSLvlVisited[10];
	char gap20F[9];
	int _pGFXLoad;
	int _pNAnim[8]; // unsigned char *
	int _pNFrames;
	int _pNWidth;
	int _pWAnim[8]; // unsigned char *
	int _pWFrames;
	int _pWWidth;
	int _pAAnim[8]; // unsigned char *
	int _pAFrames;
	int _pAWidth;
	int _pAFNum;
	int _pLAnim[8]; // unsigned char *
	int _pFAnim[8]; // unsigned char *
	int _pTAnim[8]; // unsigned char *
	int _pSFrames;
	int _pSWidth;
	int _pSFNum;
	int _pHAnim[8]; // unsigned char *
	int _pHFrames;
	int _pHWidth;
	int _pDAnim[8]; // unsigned char *
	int _pDFrames;
	int _pDWidth;
	int _pBAnim[8]; // unsigned char *
	int _pBFrames;
	int _pBWidth;
	ItemStruct InvBody[7];
	ItemStruct InvList[40];
	int _pNumInv;
	char InvGrid[40];
	ItemStruct SpdList[8];
	ItemStruct HoldItem;
	int _pIMinDam;
	int _pIMaxDam;
	int _pIAC;
	int _pIBonusDam;
	int _pIBonusToHit;
	int _pIBonusAC;
	int _pIBonusDamMod;
	int remove_2;
	int _pISpells[2]; //__declspec(align(8))
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
	unsigned char pManaShield;
	char bReserved[3];
	short wReserved[8];
	int pDiabloKillLevel;
	int dwReserved[7];
	void *_pNData;
	void *_pWData;
	void *_pAData;
	void *_pLData;
	void *_pFData;
	void *_pTData;
	void *_pHData;
	void *_pDData;
	void *_pBData;
	int unused_54D4;
};

struct ObjectStruct
{
	int _otype;
	int _ox;
	int _oy;
	int _oLight;
	int _oAnimFlag;
	int _oAnimData; // unsigned char *
	int _oAnimDelay;
	int _oAnimCnt;
	int _oAnimLen;
	int _oAnimFrame;
	int _oAnimWidth;
	int _oAnimWidth2;
	int _oDelFlag;
	int _oBreak;
	int _oSolidFlag;
	int _oMissFlag;
	int _oSelFlag;
	int _oPreFlag;
	int _oTrapFlag;
	int _oDoorFlag;
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
};

struct TriggerStruct
{
	int _tx;
	int _ty;
	int _tmsg;
	int _tlvl;
};