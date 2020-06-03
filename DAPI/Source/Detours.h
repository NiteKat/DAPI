#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

void PlaceDetour(DWORD dwAddressToPatch, DWORD dwDetourAddress, DWORD dwPadSize, BOOL bFlag);