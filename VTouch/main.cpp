#include "Utils.h"
#include "InputServer.h"

//FILE* g_LogFileRevive = NULL;

typedef FARPROC(__stdcall* _GetProcAddress)(HMODULE hModule, LPCSTR lpProcName);

_GetProcAddress TrueProcAddress;
_GetTrackingState g_TrampolineFuncAddress;

WCHAR vtModuleName[MAX_PATH];
WCHAR ovrModuleName[MAX_PATH];

FARPROC WINAPI HookProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	
	if (strncmp(lpProcName, "ovr_GetTrackingState", MAX_PATH) == 0)
	{
		std::string revProcName = lpProcName;
		revProcName.replace(0, 3, "vt");
		g_TrampolineFuncAddress = reinterpret_cast<_GetTrackingState>(TrueProcAddress(hModule, lpProcName));
		return TrueProcAddress(GetModuleHandle(vtModuleName), revProcName.c_str());
	}
	if (strncmp(lpProcName, "ovr_GetInputState", MAX_PATH) == 0)
	{
		std::string revProcName = lpProcName;
		revProcName.replace(0, 3, "vt");
		return TrueProcAddress(GetModuleHandle(vtModuleName), revProcName.c_str());
	}
	return TrueProcAddress(hModule, lpProcName);
}

//void CreateLogFile()
//{
//	WCHAR LogPath[MAX_PATH];
//	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, LogPath)))
//	{
//		wcsncat(LogPath, L"\\Revive", MAX_PATH);
//
//		BOOL exists = PathFileExists(LogPath);
//		if (!exists)
//			exists = CreateDirectory(LogPath, NULL);
//
//		wcsncat(LogPath, L"\\Revive.txt", MAX_PATH);
//		if (exists)
//			g_LogFileRevive = _wfopen(LogPath, L"w");
//		fprintf(g_LogFileRevive, "Log file created: %ws\n", LogPath);
//	}
//}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	InputServer* inputServer = InputServer::GetInstance();

#if defined(_WIN64)
	const char* pBitDepth = "64";
#else
	const char* pBitDepth = "32";
#endif
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			GetModuleFileName((HMODULE)hModule, vtModuleName, MAX_PATH);
			swprintf(ovrModuleName, MAX_PATH, L"LibOVRRT%hs_%d.dll", pBitDepth, OVR_MAJOR_VERSION);

			MH_Initialize();
			MH_CreateHook(GetProcAddress, HookProcAddress, reinterpret_cast<LPVOID*>(&TrueProcAddress));

			MH_QueueEnableHook(GetProcAddress);
			MH_ApplyQueued();

			//Create a server thread
			_beginthread(&InputServer::StartWrapper, 0, static_cast<void*>(inputServer));

			break;
		case DLL_PROCESS_DETACH:
			MH_Uninitialize();

			MH_QueueDisableHook(GetProcAddress);
			MH_ApplyQueued();

			inputServer->Run = FALSE;
			//End the server thread
			_endthread();

			break;
		default:
			break;
	}

	return TRUE;
}
