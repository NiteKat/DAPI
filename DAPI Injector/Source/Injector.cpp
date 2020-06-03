#include "Injector.h"

int inject()
{
  bool windowFound = true;
  auto gameWindow = FindWindowA(NULL, "Diablo");
  if (!gameWindow)
    return 1;

  auto dll_path = "DAPI.dll";
  DWORD processID;
  DWORD hLibModule;
  GetWindowThreadProcessId(gameWindow, &processID);
  if (!processID)
    return 2;

  auto process = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
  if (!process)
    return 3;

  auto vae_return = VirtualAllocEx(process, NULL, sizeof(dll_path), MEM_COMMIT, PAGE_READWRITE);
  if (!vae_return)
    return 4;

  if (!WriteProcessMemory(process, vae_return, (void*)dll_path, sizeof(dll_path), NULL))
    return 5;
  
  auto hKernel32 = GetModuleHandleA("Kernel32");
  HANDLE thread;
  thread = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryA"), vae_return, 0, NULL);
  WaitForSingleObject(thread, INFINITE);
  GetExitCodeThread(thread, &hLibModule);

  CloseHandle(thread);
  VirtualFreeEx(process, vae_return, sizeof(dll_path), MEM_RELEASE);
  return 0;
}