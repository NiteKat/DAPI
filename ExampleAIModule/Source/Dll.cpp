#include<Windows.h>
#include"ExampleAIModule.h"

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
	static HANDLE hEvent = nullptr;
	static HANDLE hPersistThread = nullptr;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_DETACH:
		break;
	case DLL_PROCESS_ATTACH:
		break;
	}
	return TRUE;
}