#include "InputServer.h"

InputServer* InputServer::m_Instance = 0;

InputServer::InputServer()
	: Run(TRUE)
	, m_IsProcessing(FALSE)
	, m_ClientSocket(INVALID_SOCKET)
	, m_ListenSocket(INVALID_SOCKET)
	, m_Recvbuf{ '0' }
	, g_InputManager(InputManager::GetInstance())
{
	WCHAR LogPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, LogPath)))
	{
		wcsncat(LogPath, L"\\Revive", MAX_PATH);

		BOOL exists = PathFileExists(LogPath);
		if (!exists)
			exists = CreateDirectory(LogPath, NULL);

		wcsncat(LogPath, L"\\ReviveServerLog.txt", MAX_PATH);
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
	int iResult = 0;
	int iSendResult = 0;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	WSADATA wsaData;
	SOCKET m_ListenSocket;

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
	m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_ListenSocket == INVALID_SOCKET) {
		LogServer("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return FALSE;
	}

	// Setup the TCP listening socket
	iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		LogServer("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(m_ListenSocket);
		WSACleanup();
		return FALSE;
	}

	freeaddrinfo(result);

	iResult = listen(m_ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		LogServer("listen failed with error: %d\n", WSAGetLastError());
		closesocket(m_ListenSocket);
		WSACleanup();
		return FALSE;
	}

	while (Run)
	{
		if (m_IsProcessing)
			continue;
		// Accept a client socket
		m_ClientSocket = accept(m_ListenSocket, NULL, NULL);
		if (m_ClientSocket == INVALID_SOCKET) {
			LogServer("accept failed with error: %d\n", WSAGetLastError());
			closesocket(m_ListenSocket);
			WSACleanup();
			return FALSE;
		}
		LogServer("A client connected.\n");

	//do
	//{
		iResult = recv(m_ClientSocket, m_Recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			LogServer("Message received: %s\n", m_Recvbuf);
			m_MessageProcess();
			// Echo the buffer back to the sender
			iSendResult = send(m_ClientSocket, m_Recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				LogServer("send failed with error: %d\n", WSAGetLastError());
				//closesocket(m_ClientSocket);
				//WSACleanup();
				//return FALSE;
			}
			//LogServer("Bytes sent: %d\n", iSendResult);
		}
		//else if (iResult == 0) {
		//	LogServer("Connection closing...\n");
		//}
		//else {
		//	LogServer("recv failed with error: %d\n", WSAGetLastError());
		//	//closesocket(m_ClientSocket);
		//	//WSACleanup();
		//	//return FALSE;
		//}

	//} while (iResult > 0);
	}

	closesocket(m_ListenSocket);

	// shutdown the connection since we're done
	iResult = shutdown(m_ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		LogServer("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(m_ClientSocket);
		WSACleanup();
		return FALSE;
	}

	// cleanup
	closesocket(m_ClientSocket);
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
	std::regex input("input");
	std::regex reset("reset");
	std::regex digit("\\b\\d+\\.*\\d*");
	std::smatch m;
	std::vector<std::string> keys;

	if (std::regex_search(s, position))
	{
		while (std::regex_search(s, m, digit)) {
			keys.push_back(m.str().c_str());
			s = m.suffix().str();
		}
		if (keys.size() == 3)
		{
			//g_InputManager->EmulateTouchesPositionOffset((unsigned int)atoi(keys[0].c_str()), (float)atof(keys[1].c_str()), (float)atof(keys[2].c_str()));
		}
	}
	else if (std::regex_search(s, input))
	{
		while (std::regex_search(s, m, digit)) {
			keys.push_back(m.str());
			s = m.suffix().str();
		}
		if (keys.size() == 5)
		{			
			g_InputManager->EmulateTouchesInputState((unsigned int)atoi(keys[0].c_str()), (bool)atoi(keys[1].c_str()), (float)atof(keys[2].c_str()), (float)atof(keys[3].c_str()), (float)atof(keys[4].c_str()));
		}
	}
	else if (std::regex_search(s, reset))
	{
		LogServer("pose reset\n");
		g_InputManager->EmulateResetTouchesPose();
	}
	
	m_IsProcessing = FALSE;
	return TRUE;
}

