// Twink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <winbase.h>
#include <synchapi.h>

const int SECONDS_IN_HOUR = 3600;
const int MILI_MULTIPLIER = 1000;
const int MAX_PATH_SIZE = 260;
const char AUTORUNS[46] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* MUTEXNAME = "visual_studio_haram";
const LPCSTR WELCOME_WINDOW_TITLE = "Management program is up ( sadly it was developed in visual studio )";
const LPCSTR WELCOME_WINDOW_TEXT = "Visual Studio made me sad";
const char* AUTORUN_VALUE_NAME = "Technician";

HANDLE ghMutex;

int ShowPopup() {
	int msgboxID = MessageBox(
		NULL,
		WELCOME_WINDOW_TITLE,
		WELCOME_WINDOW_TEXT,
		MB_ICONINFORMATION
	);
	return msgboxID;
}

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


int main()
{
	ghMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEXNAME);
	if (ghMutex != NULL) {
		std::cerr << "Already open";
		return 1;
	}

	ghMutex = CreateMutexA(
		NULL,
		TRUE,
		MUTEXNAME);

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}

	int msgboxID = ShowPopup();

	HKEY TargetKey;
	LSTATUS OpenStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &TargetKey);
	if (TargetKey == NULL) {
		std::cout << "Failed to create key" << std::endl;
		return 1;
	}
	CHAR my_str[MAX_PATH_SIZE];
	LPSTR CurrentPath = my_str; 
	GetModuleFileNameA(NULL, CurrentPath, MAX_PATH_SIZE);
	append_gershaim(my_str);
	RegSetValueExA(TargetKey, TEXT(AUTORUN_VALUE_NAME), 0, REG_SZ, (BYTE*)my_str, MAX_PATH_SIZE);

	Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

	CloseHandle(ghMutex);
} 











