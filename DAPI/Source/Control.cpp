#include"Control.h"
namespace DAPI
{

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