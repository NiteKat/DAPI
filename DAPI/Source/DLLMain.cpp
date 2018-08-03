#include<Windows.h>
#include<stdio.h>
#include"Globals.h"
#include"Detours.h"
#include"../Common/Structs.h"
#include"Control.h"
#include"../Common/Game.h"
//#include"../Interface/AIModule.h"
#include<math.h>
#include<set>

HMODULE AIHandle = NULL;
void(*onFrame)(DAPI::Game& Diablo) = nullptr;
void(*onDraw)() = nullptr;
DAPI::Game Diablo;
typedef unsigned __int32 u32;
static const u32 hook_function = 0x408A06;//0x408974;//0x46886B;
static const u32 hook_function2 = 0x4565F0;
int hook1 = 0;
int hook2 = 0;
int hook_count = 0;
int times_greater_1 = 0;


void __declspec(noinline) updateGameData()
{
	if (!AIHandle)
	{
		//load bot
		AIHandle = LoadLibraryA("ExampleAIModule.dll");
		if (AIHandle)
		{
			onFrame = reinterpret_cast<void(*)(DAPI::Game& Diablo)>(GetProcAddress(AIHandle, "onFrame"));
		}
	}
}

void __declspec(noinline) runBot()
{
	onFrame(Diablo);
}

[[noreturn]] __declspec(naked) void trampoline() {
	//Save Registers
	__asm { pushad }

	{
		//Enter trampoline
		__asm {
			push ebp
			mov ebp, esp
		}
		
		updateGameData();
		runBot();

		//Simulate return
		__asm {
			pop ebp
		}
	}

	__asm { popad }

	static auto game_loop = reinterpret_cast<void(__fastcall *)(bool startup)>(0x40AAE3);
	static auto target = reinterpret_cast<void*>(0x408A0B);

	__asm {
		call game_loop
		jmp target
	}
	/*
	// Enter trampoline
	__asm {
		push esi
		push eax 
		push ebp
		mov ebp, esp
	}
	
	updateGameData();

	runBot();

	hook_count++;
	hook1++;

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
	*/
}

[[noreturn]] __declspec(naked) void trampoline2() {
	//Save registers
	__asm { pushad }

	{
		//Enter trampoline
		__asm {
			push ebp
			mov ebp, esp
		}
		hook2++;
		if (hook_count > 1)
			times_greater_1++;
		hook_count = 0;
		Diablo.onDraw();

		//Simulate return
		__asm {
			pop ebp
		}
	}

	__asm { popad }

	static auto DrawMain = reinterpret_cast<void(__fastcall *)(int dwHgt, int draw_desc, int draw_hp, int draw_mana, int draw_sbar, int draw_btn)>(0x456124);

	__asm {
		call DrawMain
		mov edx, 0x004565F5
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
		PlaceDetour(hook_function, (DWORD)trampoline, 0, true);
		PlaceDetour(hook_function2, (DWORD)trampoline2, 0, true);
		break;
	}
	return TRUE;
}