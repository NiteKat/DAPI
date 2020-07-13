#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

void PlaceDetour(DWORD dwAddressToPatch, DWORD dwDetourAddress, DWORD dwPadSize, BOOL bFlag);

void Patch(DWORD dwAddressToPatch, std::string assem, DWORD dwPadSize);