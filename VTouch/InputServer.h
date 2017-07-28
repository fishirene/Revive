#pragma once

#include "Utils.h"
#include "InputManager.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#define LogServer(x, ...) if (m_ServerLog) fprintf(m_ServerLog, x, __VA_ARGS__); \
					printf(x, __VA_ARGS__); \
					fflush(m_ServerLog);

class InputServer
{
public:
	~InputServer();
	static InputServer* GetInstance();

	bool Run;

	static void __cdecl StartWrapper(void* p);

private:
	InputServer();
	static InputServer* m_Instance;

	InputManager* g_InputManager;
	FILE* m_ServerLog;
	
	char m_Recvbuf[DEFAULT_BUFLEN];
	bool m_IsProcessing;

	unsigned int m_Start();

	bool m_MessageProcess();
};
