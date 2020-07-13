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

void Patch(DWORD dwAddressToPatch, std::string assem, DWORD dwPadSize)
{
  DWORD dwOldProtect = NULL;
  VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), PAGE_EXECUTE_READWRITE, &dwOldProtect);
  for (int i = 0; i < assem.size(); i++)
    *(BYTE*)(dwAddressToPatch + i) = assem[i];
  VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), dwOldProtect, &dwOldProtect);
}