#include "Detours.h"

void PlaceDetour(DWORD dwAddressToPatch, DWORD dwDetourAddress, DWORD dwPadSize, BOOL bFlag) {

	DWORD dwOldProtect = NULL;

	VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), PAGE_EXECUTE_READWRITE, &dwOldProtect);

	if (bFlag) {
		*(BYTE*)(dwAddressToPatch) = 0xE9;
	}
	else {
		*(BYTE*)(dwAddressToPatch) = 0xE8;
	}
	*(DWORD*)(dwAddressToPatch + 1) = ((dwDetourAddress - dwAddressToPatch) - 5);
	for (DWORD i = dwPadSize; i > 0; i--) {
		*(BYTE*)(dwAddressToPatch + 5 + i) = 0x90;
	}

	VirtualProtect((LPVOID)dwAddressToPatch, (dwPadSize + 5), dwOldProtect, &dwOldProtect);

}

void PatchMemory(DWORD dwAddressToPatch, BYTE *pData, DWORD dwDataLength) {

	DWORD dwOldProtect = NULL;

	VirtualProtect((LPVOID)dwAddressToPatch, dwDataLength, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	for (DWORD i = 0; i < dwDataLength; i++) {
		*(BYTE*)(dwAddressToPatch + (i)) = pData[i];
	}

	VirtualProtect((LPVOID)dwAddressToPatch, dwDataLength, dwOldProtect, &dwOldProtect);

}