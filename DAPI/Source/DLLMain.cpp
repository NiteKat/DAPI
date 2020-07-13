#include <stdio.h>

#include "Detours.h"
#include "Game.h"

#include <Windows.h>

typedef unsigned __int32 u32;

static const u32 hook_function = 0x408A06;
static const u32 speed_offset = 0x441219;

DAPI::Game Diablo;

HANDLE CreateUniqueEvent()
{
  static char szEventName[32];
  sprintf_s(szEventName, "DAPI #%u", GetCurrentProcessId());

  HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, szEventName);
  if (GetLastError() == ERROR_ALREADY_EXISTS)
    hEvent = nullptr;

  return hEvent;
}

void __declspec(noinline) updateGameData()
{
  return;
}

[[noreturn]] __declspec(naked) void trampoline()
{
  //Save Registers
  __asm { pushad }

  {
    //Enter trampoline
    __asm {
      push ebp
      mov ebp, esp
    }

    // TODO add server code here
    Diablo.server.update();

    //Simulate return
    __asm {
      pop ebp
    }
  }

  __asm { popad }

  static auto game_loop = reinterpret_cast<void(__fastcall*)(bool startup)>(0x40AAE3);
  static auto target = reinterpret_cast<void*>(0x408A0B);

  __asm {
    call game_loop
    jmp target
  }
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
    Patch(speed_offset, "\xB0\x01\x90", 0);
    break;
  }
  return TRUE;
}