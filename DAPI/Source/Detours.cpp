#include "Detours.h"

void PlaceDetour(DWORD dwAddressToPatch, DWORD dwDetourAddress, DWORD dwPadSize, BOOL bFlag)
{
  DWORD dwOldProtect = NULL;

  VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), PAGE_EXECUTE_READWRITE, &dwOldProtect);

  if (bFlag) {
    *(BYTE*)(dwAddressToPatch) = 0xE9;
  }
  else
  {
    *(BYTE*)(dwAddressToPatch) = 0XE8;
  }
  *(DWORD*)(dwAddressToPatch + 1) = ((dwDetourAddress - dwAddressToPatch) - 5);
  for (DWORD i = dwPadSize; i > 0; i--)
  {
    *(BYTE*)(dwAddressToPatch + 5 + i) = 0x90;
  }

  VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), dwOldProtect, &dwOldProtect);
}

void Patch(char* dst, const char* src, int size)
{
  //return;
  DWORD oldprotect;
  VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
  memcpy(dst, src, size);
  VirtualProtect(dst, size, oldprotect, &oldprotect);
}