#include<Windows.h>
#include<stdio.h>
#include"Globals.h"
#include"Detours.h"
#include"../Common/Structs.h"
#include"Control.h"
//#include"../Interface/AIModule.h"
#include<math.h>
#include<set>

DAPI::AIModule* module = nullptr;
HMODULE AIHandle = NULL;
void(*onFrame)() = nullptr;

void __declspec(noinline) updateGameData()
{
	if (!AIHandle)
	{
		//load bot
		AIHandle = LoadLibraryA("ExampleAIModule.dll");
		if (AIHandle)
			onFrame = reinterpret_cast<void(*)()>(GetProcAddress(AIHandle, "onFrame"));
	}
}

void __declspec(noinline) runBot()
{
	onFrame();
}

[[noreturn]] __declspec(naked) void trampoline() {
	// Enter trampoline
	__asm {
		push esi
		push eax 
		push ebp
		mov ebp, esp
	}
	
	updateGameData();

	runBot();

	// Simulate return
	__asm {
		pop ebp
		pop eax
		pop esi
	}

	// Execute overwritten instructions
	__asm {
		push esi
		push esi
		push esi
		push esi
		push eax
	}

	// Jump back to the rest of the function
	__asm {
		mov edx, 0x00408979
		jmp edx
	}
	
}

HANDLE CreateUniqueEvent()
{
	static char szEventName[32];
	sprintf_s(szEventName, "DAPI #%u", GetCurrentProcessId());

	HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, szEventName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		hEvent = nullptr;

	return hEvent;
}

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
	static HANDLE hEvent = nullptr;
	static HANDLE hPersistThread = nullptr;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_DETACH:
		if (hEvent != nullptr)
			CloseHandle(hEvent);
		if (hPersistThread != nullptr)
			CloseHandle(hPersistThread);
		break;
	case DLL_PROCESS_ATTACH:
		hEvent = CreateUniqueEvent();
		if (!hEvent)
			return FALSE;
		auto process = GetCurrentProcess();
		PlaceDetour(DAPI::hook_function, (DWORD)trampoline, 0, true);
		break;
	}
	return TRUE;
}