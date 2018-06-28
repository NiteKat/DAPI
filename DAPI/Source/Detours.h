#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

void PlaceDetour(DWORD dwAddressToPatch, DWORD dwDetourAddress, DWORD dwPadSize, BOOL bFlag);
void PatchMemory(DWORD dwAddressToPatch, BYTE *pData, DWORD dwDataLength);