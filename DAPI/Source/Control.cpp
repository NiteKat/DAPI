#include"Control.h"
namespace DAPI
{
	__declspec(dllexport) void walk(int x, int y)
	{
		ClrPlrPath(0);
		MakePlrPath(0, x, y, 1u);
		(*player)[0].destAction = -1;
		//DAPI::NetSendCmdLoc(1u, DAPI::_cmd_id::CMD_WALKXY, unsigned char(x), unsigned char(y));
	}

	__declspec(dllexport) void attack(int x, int y)
	{
		MakePlrPath(0, (unsigned char)x, (unsigned char)y, 0);
		(*player)[0].destAction = 9;
		(*player)[0].destParam1 = x;
		(*player)[0].destParam2 = y;
	}

	__declspec(dllexport) void useItem(int i)
	{
		if (6 < i && i < 47)
		{
			if ((*player)[0].InvList[i - 7]._itype != -1)
				UseInvItem(0, i);
		}
		else if (46 < i && i < 57)
		{
			if ((*player)[0].SpdList[i - 47]._itype != -1)
				UseInvItem(0, i);
		}
	}

	__declspec(dllexport) void rangeAttack(int x, int y)
	{
		ClrPlrPath(0);
		(*player)[0].destAction = 10;
		(*player)[0].destParam1 = x;
		(*player)[0].destParam2 = y;
	}

	__declspec(dllexport) void castSpell(int x, int y)
	{
		if ((*player)[0]._pRSpell <= spell_id::SPL_BONESPIRIT &&
			spell_id::SPL_NULL < (*player)[0]._pRSpell)
		{
			if ((*player)[0]._pRSplType == spell_type::RSPLTYPE_SPELL)
			{
				if (CheckSpell(0, (*player)[0]._pRSpell, (*player)[0]._pRSplType, 0))
				{
					ClrPlrPath(0);
					(*player)[0].destAction = 12;
					(*player)[0].destParam1 = (unsigned char)x;
					(*player)[0].destParam2 = (unsigned char)y;
					(*player)[0].destParam3 = GetSpellLevel(0, (*player)[0]._pRSpell);
					(*player)[0]._pSplFrom = 0;
					(*player)[0]._pSpell = (*player)[0]._pRSpell;
					(*player)[0]._pSplType = (*player)[0]._pRSplType;
				}
			}
			else if ((*player)[0]._pRSplType == spell_type::RSPLTYPE_CHARGES)
			{
				ClrPlrPath(0);
				(*player)[0].destAction = 12;
				(*player)[0].destParam1 = x;
				(*player)[0].destParam2 = y;
				(*player)[0].destParam3 = GetSpellLevel(0, (*player)[0]._pRSpell);
				(*player)[0]._pSplFrom = 0;
				(*player)[0]._pSpell = (*player)[0]._pRSpell;
				(*player)[0]._pSplType = (*player)[0]._pRSplType;
			}
			else if ((*player)[0]._pRSplType == spell_type::RSPLTYPE_SCROLL)
			{
				if (UseScroll())
				{
					ClrPlrPath(0);
					(*player)[0].destAction = 12;
					(*player)[0].destParam1 = x;
					(*player)[0].destParam2 = y;
					(*player)[0].destParam3 = GetSpellLevel(0, (*player)[0]._pRSpell);
					(*player)[0]._pSplFrom = 0;
					(*player)[0]._pSpell = (*player)[0]._pRSpell;
					(*player)[0]._pSplType = (*player)[0]._pRSplType;
				}
			}
		}
	}

	__declspec(dllexport) void interactObject(ObjectStruct* target_object)
	{
		if (target_object->_oSolidFlag || target_object->_oDoorFlag)
			MakePlrPath(0, (unsigned char)target_object->_ox, (unsigned char)target_object->_oy, 0);
		else
			MakePlrPath(0, (unsigned char)target_object->_ox, (unsigned char)target_object->_oy, 1u);
		(*player)[0].destAction = 13;
		for (int i = 0; i < 127; i++)
		{
			if (&(*object)[i] == target_object)
			{
				(*player)[0].destParam1 = i;
				break;
			}
		}
	}

	__declspec(dllexport) bool isClosed(ObjectStruct* target_object)
	{
		return !target_object->_oVar4 && target_object->_oDoorFlag;
	}

	__declspec(dllexport) void pickupItem(ItemStruct* target_item)
	{
		MakePlrPath(0, (unsigned char)target_item->_ix, (unsigned char)target_item->_iy, 0);
		(*player)[0].destAction = 16;
		for (int i = 0; i < 127; i++)
		{
			if (&(*item)[i] == target_item)
			{
				(*player)[0].destParam1 = i;
				break;
			}
		}
	}

	__declspec(dllexport) PlayerStruct* getPlayer(int pnum)
	{
		return &((*player)[0]);
	}
}