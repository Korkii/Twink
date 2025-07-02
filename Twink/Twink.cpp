// Twink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <winbase.h>
#include <synchapi.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

const int SECONDS_IN_HOUR = 3600;
const int MILI_MULTIPLIER = 1000;
const int MAX_PATH_SIZE = 260;
const char AUTORUNS[46] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* MUTEXNAME = "visual_studio_haram";
const LPCSTR WELCOME_WINDOW_TITLE = "Management program is up ( sadly it was developed in visual studio )";
const LPCSTR WELCOME_WINDOW_TEXT = "Visual Studio made me sad";
const char* AUTORUN_VALUE_NAME = "Technician";

HANDLE ghMutex;

enum error_code {
	SUCCESS,
	ALREADY_OPEN,
	UNKNOWN,
};

/*
Shows the popup

@returns status code
*/
int ShowPopup() {
	int msgboxID = MessageBox(
		NULL,
		WELCOME_WINDOW_TITLE,
		WELCOME_WINDOW_TEXT,
		MB_ICONINFORMATION
	);
	return msgboxID;
}


/*
Legit appends gershaim.

@param my_str The null terminated string to append garshiaim to
*/
void append_gershaim(CHAR* my_str) {
	CHAR curr;
	CHAR prev = my_str[0];
	int i;
	for (i = 1; my_str[i - 1] != '\0'; i++) {
		curr = my_str[i];
		my_str[i] = prev;
		prev = curr;
	}
	my_str[0] = '"';
	my_str[i - 1] = '"';
	my_str[i] = '\0';
}


/*
Handles all error types

@param errorCode Error code to take care of
@return errorcode
*/
int handle_errors(error_code errorCode) {
	switch (errorCode) {
	case error_code::ALREADY_OPEN:
		std::cerr << "Already open";
		return errorCode;
	}

	return error_code::UNKNOWN;
}


int main()
{
	try {
	ghMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEXNAME);
	if (ghMutex != NULL) {
		throw error_code::ALREADY_OPEN;
	}
	ghMutex = CreateMutexA(
		NULL,
		TRUE,
		MUTEXNAME);

	int msgboxID = ShowPopup();

	HKEY TargetKey;
	LSTATUS OpenStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &TargetKey);
	CHAR my_str[MAX_PATH_SIZE];
	LPSTR CurrentPath = my_str; 
	GetModuleFileNameA(NULL, CurrentPath, MAX_PATH_SIZE);
	append_gershaim(my_str);
	RegSetValueExA(TargetKey, TEXT(AUTORUN_VALUE_NAME), 0, REG_SZ, (BYTE*)my_str, MAX_PATH_SIZE);


	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}


	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

	CloseHandle(ghMutex);

	return error_code::SUCCESS;
	}
	catch (error_code errorCode) {
		return handle_errors(errorCode);
		
	}
} 











