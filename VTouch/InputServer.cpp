#include "InputServer.h"

InputServer* InputServer::m_Instance = 0;

InputServer::InputServer()
	: Run(TRUE)
	, m_IsProcessing(FALSE)
	, m_Recvbuf{ '0' }
	, g_InputManager(InputManager::GetInstance())
{
	WCHAR LogPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, LogPath)))
	{
		wcsncat(LogPath, L"\\VTouch", MAX_PATH);

		BOOL exists = PathFileExists(LogPath);
		if (!exists)
			exists = CreateDirectory(LogPath, NULL);

		wcsncat(LogPath, L"\\VTouchServerLog.txt", MAX_PATH);
		if (exists)
			m_ServerLog = _wfopen(LogPath, L"w");
	}
}

InputServer::~InputServer()
{
	delete m_Instance;
}

InputServer* InputServer::GetInstance() {
	if (!m_Instance) {
		m_Instance = new InputServer();
		return m_Instance;
	}
	else {
		return m_Instance;
	}
}

void __cdecl InputServer::StartWrapper(void* p)
{
	static_cast<InputServer*>(p)->m_Start();
}

unsigned int InputServer::m_Start() {
	memset(m_Recvbuf, 0, sizeof(m_Recvbuf));

	int iResult = 0;
	int iSendResult = 0;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	WSADATA wsaData;
	SOCKET listenSocket;
	SOCKET clientSocket;

	LogServer("Server Started.\n");

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		LogServer("WSAStartup failed with error: %d\n", iResult);
		return FALSE;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		LogServer("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return FALSE;
	}

	// Create a SOCKET for connecting to server
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		LogServer("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return FALSE;
	}

	// Setup the TCP listening socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		LogServer("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return FALSE;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		LogServer("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return FALSE;
	}

	while (Run)
	{
		if (m_IsProcessing)
			continue;

		// Accept a client socket
		clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			LogServer("accept failed with error: %d\n", WSAGetLastError());
		}
		LogServer("A client connected.\n");

		iResult = recv(clientSocket, m_Recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			LogServer("Message received: %s\n", m_Recvbuf);
			m_MessageProcess();
			// Echo the buffer back to the sender
			iSendResult = send(clientSocket, m_Recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				LogServer("send failed with error: %d\n", WSAGetLastError());
			}
			//LogServer("Feedback sent: %s\n", m_Recvbuf);
		}
		iResult = shutdown(clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			LogServer("shutdown failed with error: %d\n", WSAGetLastError());
		}
		LogServer("A client disconnected.\n");
	}

	closesocket(listenSocket);

	// shutdown the connection since we're done
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		LogServer("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return FALSE;
	}

	// cleanup
	closesocket(clientSocket);
	WSACleanup();

	return TRUE;
}

bool InputServer::m_MessageProcess()
{
	m_IsProcessing = TRUE;

	unsigned int touchKey = 0;
	bool state = 0;
	float value = 0, x = 0, y = 0;

	std::string s = m_Recvbuf;
	std::regex position("position");
	std::regex rotation("rotation");
	std::regex button("button");
	std::regex stick("stick");
	std::regex trigger("trigger");
	std::regex reset("reset");
	std::regex digit("-?\\b\\d+\\.?\\d*");
	std::smatch m;
	std::vector<std::string> keys;

	while (std::regex_search(s, m, digit)) {
		keys.push_back(m.str());
		s = m.suffix().str();
	}

	if (std::regex_search(m_Recvbuf, position))
	{
		if (keys.size() % 3 == 0)
		{
			LogServer("Position set.\n");
			while (!keys.empty())
			{
				g_InputManager->EmulateTouchesPositionOffset((unsigned int)atoi(keys[0].c_str()), (float)atof(keys[1].c_str()), (float)atof(keys[2].c_str()));
				keys.erase(keys.begin(), keys.begin() + 3);
			}
		}
	}
	else if (std::regex_search(m_Recvbuf, rotation))
	{
		if (keys.size() % 3 == 0)
		{
			LogServer("Rotation set.\n");
			while (!keys.empty())
			{
				g_InputManager->EmulateTouchesOrientationOffset((unsigned int)atoi(keys[0].c_str()), (unsigned int)atoi(keys[1].c_str()), (float)atof(keys[2].c_str()));
				keys.erase(keys.begin(), keys.begin() + 3);
			}
		}
	}
	else if (std::regex_search(m_Recvbuf, trigger))
	{
		if (keys.size() % 2 == 0)
		{
			LogServer("Trigger set.\n");
			while (!keys.empty())
			{
				g_InputManager->EmulateTouchesInputState((unsigned int)atoi(keys[0].c_str()), (float)atof(keys[1].c_str()));
				keys.erase(keys.begin(), keys.begin() + 2);
			}
		}
	}
	else if (std::regex_search(m_Recvbuf, button))
	{
		if (keys.size() % 2 == 0)
		{
			LogServer("Button set.\n");
			while (!keys.empty())
			{
				g_InputManager->EmulateTouchesInputState((unsigned int)atoi(keys[0].c_str()), (bool)atoi(keys[1].c_str()));
				keys.erase(keys.begin(), keys.begin() + 2);
			}
		}
	}
	else if (std::regex_search(m_Recvbuf, stick))
	{
		if (keys.size() % 3 == 0)
		{
			LogServer("Stick set.\n");
			while (!keys.empty())
			{
				g_InputManager->EmulateTouchesInputState((unsigned int)atoi(keys[0].c_str()), (float)atof(keys[1].c_str()), (float)atof(keys[2].c_str()));
				keys.erase(keys.begin(), keys.begin() + 3);
			}
		}
	}
	else if (std::regex_search(m_Recvbuf, reset))
	{
		LogServer("Pose reset.\n");
		g_InputManager->EmulateResetTouchesPose();
	}
	
	memset(m_Recvbuf, 0, sizeof(m_Recvbuf));

	m_IsProcessing = FALSE;
	return TRUE;
}

