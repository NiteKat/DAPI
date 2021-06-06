#include <stdio.h>

#include "Detours.h"
#include "Game.h"

#include <Windows.h>

typedef unsigned __int32 u32;

static const u32 hook_function = 0x408A06;
static const u32 speed_offset = 0x4411EF;
static const u32 mp_speed_offset = 0x440EAA;
static const u32 mp_speed_offset2 = 0x4410CF;

DAPI::Game Diablo;

bool speedHackResult;
bool mpSpeedHackResult;
BOOL *mpSpeedHackBOOL;

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

BOOL new_nthread_has_500ms_passed()
{
  auto gbMaxPlayers = reinterpret_cast<int(*)>(0x679660);
  auto nthread_has_500ms_passed = reinterpret_cast<BOOL(__fastcall*)()>(0x4411EF);
  auto last_tick = reinterpret_cast<int(*)>(0x679764);

  DWORD currentTickCount;
  int ticksElapsed;

  currentTickCount = GetTickCount();
  ticksElapsed = currentTickCount - *last_tick;
  if (*gbMaxPlayers == 1 && ticksElapsed > (1000 / Diablo.server.FPS) * 10 ) {
    *last_tick = currentTickCount;
    ticksElapsed = 0;
  }
  return ticksElapsed >= 0;
}

BOOL new_nthread_recv_turns(BOOL* pfSendAsync)
{
  auto sgbPacketCountdown = reinterpret_cast<char(*)>(0x679759);
  auto sgbSyncCountdown = reinterpret_cast<char(*)>(0x679750);
  auto sgbNetUpdateRate = reinterpret_cast<BYTE(*)>(0x679704);
  auto SNetReceiveTurns = reinterpret_cast<BOOL(__stdcall*)(int, int, char**, DWORD*, DWORD*)>(0x4698BE);
  auto glpMsgTbl = reinterpret_cast<int(*)>(0x67973C);
  auto gdwMsgLenTbl = reinterpret_cast<DWORD(*)>(0x679708);
  auto player_state = reinterpret_cast<int(*)>(0x6796F0);
  auto SErrGetLastError = reinterpret_cast<DWORD(__stdcall*)()>(0x46985E);
  auto nthread_terminate_game = reinterpret_cast<void(__fastcall*)(const char*)>(0x440DEC);
  auto sgbTicsOutOfSync = reinterpret_cast<BOOLEAN(*)>(0x679758);
  auto multi_msg_countdown = reinterpret_cast<void(__fastcall*)()>(0x43FE0E);
  auto last_tick = reinterpret_cast<int(*)>(0x679764);

  *pfSendAsync = FALSE;
  *sgbPacketCountdown = *sgbPacketCountdown - 1;
  if (*sgbPacketCountdown) {
    *last_tick = *last_tick + 1000 / Diablo.server.FPS;
    return TRUE;
  }
  *sgbSyncCountdown = *sgbSyncCountdown - 1;
  *sgbPacketCountdown = *sgbNetUpdateRate;
  if (*sgbSyncCountdown != 0) {

    *pfSendAsync = TRUE;
    *last_tick = *last_tick + 1000 / Diablo.server.FPS;
    return TRUE;
  }
  if (!SNetReceiveTurns(0, 4, (char**)glpMsgTbl, gdwMsgLenTbl, (LPDWORD)player_state)) {
    if (SErrGetLastError() != 0x8510006b)
      nthread_terminate_game("SNetReceiveTurns");
    *sgbTicsOutOfSync = FALSE;
    *sgbSyncCountdown = 1;
    *sgbPacketCountdown = 1;
    return FALSE;
  }
  else {
    if (!*sgbTicsOutOfSync) {
      *sgbTicsOutOfSync = TRUE;
      *last_tick = GetTickCount();
    }
    *sgbSyncCountdown = 4;
    multi_msg_countdown();
    *pfSendAsync = TRUE;
    *last_tick = *last_tick + 1000 / Diablo.server.FPS;
    return TRUE;
  }
}

unsigned int __stdcall new_nthread_handler(void* data)
{
  auto nthread_should_run = reinterpret_cast<BOOLEAN(*)>(0x679734);
  auto nthread_send_and_recv_turn = reinterpret_cast<DWORD(*__fastcall)(DWORD, int)>(0x440E28);
  auto last_tick = reinterpret_cast<int(*)>(0x679764);
  

  int delta;
  BOOL received;

  if (*nthread_should_run)
  {
    while (1)
    {
      EnterCriticalSection((LPCRITICAL_SECTION)0x679718);
      if (!*nthread_should_run)
        break;
      nthread_send_and_recv_turn(0, 0);
      if (new_nthread_recv_turns(&received))
        delta = *last_tick - GetTickCount();
      else
        delta = 1000 / Diablo.server.FPS;
      LeaveCriticalSection((LPCRITICAL_SECTION)0x679718);
      if (delta > 0)
        Sleep(delta);
      if (!*nthread_should_run)
        return 0;
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)0X679718);
  }
  return 0;
}

[[noreturn]] __declspec(naked) void trampoline4()
{
  //Save Registers
  __asm { pushad }
  {
    //Enter trampoline
    __asm {
      push ebp
      mov ebp, esp
    }

    new_nthread_handler(nullptr);

    //simulate return
    __asm {
      pop ebp
    }
  }

  //restore registers
  __asm { popad }

  static auto target4 = reinterpret_cast<void(*)>(0x441142);

  __asm { 
    pop edi
    pop esi
    xor eax,eax
    pop ebx
    pop ecx
    jmp target4
  }
}

[[noreturn]] __declspec(naked) void trampoline3()
{
  //Save Registers
  __asm { pushad  }
  {
    //Enter trampoline
    __asm {
      push ebp
      mov ebp, esp
      mov mpSpeedHackBOOL, ecx
    }

    mpSpeedHackResult = new_nthread_recv_turns(mpSpeedHackBOOL);

    //Simulate return
    __asm {
      pop ebp
    }
  }

  //restore registers
  __asm { popad }

  static auto target3 = reinterpret_cast<void(*)>(0x440EC3);

  if (mpSpeedHackResult)
  {
    __asm {
      xor eax, eax
      inc eax
      pop esi
      jmp target3
    }
  }
  else
  {
    __asm {
      xor eax, eax
      pop esi
      jmp target3
    }
  }

}

[[noreturn]] __declspec(naked) void trampoline2()
{
  //Save Registers
  __asm { pushad }
  {
    //Enter trampoline
    __asm {
      push ebp
      mov ebp, esp
    }

    speedHackResult = new_nthread_has_500ms_passed();

    //Simulate return
    __asm {
      pop ebp
    }
  }

  //restore registers
  __asm { popad }

  static auto target2 = reinterpret_cast<void(*)>(0x44121C);
  if (speedHackResult)
  {
    __asm {
      mov al, 0x1
      jmp target2
    }
  }
  else
  {
    __asm {
      mov al, 0x0
      jmp target2
    }
  }
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
    //PlaceDetour(speed_offset, (DWORD)trampoline2, 0, true);
    //PlaceDetour(mp_speed_offset, (DWORD)trampoline3, 0, false);
    //PlaceDetour(mp_speed_offset2, (DWORD)trampoline4, 0, false);
    break;
  }
  return TRUE;
}