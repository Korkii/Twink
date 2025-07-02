// Twink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <winbase.h>
#include <synchapi.h>
#include <string.h>


const int SECONDS_IN_HOUR = 3600;
const int MILI_MULTIPLIER = 1000;
const int MAX_PATH_SIZE = 260;
const char AUTORUNS[46] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* MUTEXNAME = "visual_studio_good";
const LPCSTR WELCOME_WINDOW_TITLE = "Management program is up (  it was developed in visual studio happily )";
const LPCSTR WELCOME_WINDOW_TEXT = "Visual Studio made me sad";
const char* AUTORUN_VALUE_NAME = "Technician";

HANDLE g_hMutex;

enum error_code {
	SUCCESS,
	ALREADY_OPEN,
	CREATE_MUTEX_FAILED,
	CLOSE_MUTEX_FAILED,
	POPUP_FAILED,
	REGOPEN_FAILED,
	REGSET_FAILED,
	GET_MODULE_FAILED,
	UNKNOWN = 99,
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

	default:
		std::cerr << GetLastError() << std::endl;
		return errorCode;
	}
	return error_code::UNKNOWN;
}


int main(int argc, char* argv[])
{
	std::cout << argv[0] << std::endl;
	try {
	g_hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEXNAME);
	if (g_hMutex != NULL) {
		throw error_code::ALREADY_OPEN;
	}
	g_hMutex = CreateMutexA(
		NULL,
		TRUE,
		MUTEXNAME);

	if (g_hMutex == NULL) {
		throw error_code::CREATE_MUTEX_FAILED;
	}

	int msgboxID = ShowPopup();
	if (msgboxID == NULL) {
		throw error_code::POPUP_FAILED;
	}

	HKEY TargetKey;
	LSTATUS OpenStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &TargetKey);
	if (OpenStat != ERROR_SUCCESS) {
		throw error_code::REGOPEN_FAILED;
	}

	CHAR my_str[MAX_PATH_SIZE];
	LPSTR CurrentPath = my_str; 
	DWORD GetModuleStatus = GetModuleFileNameA(NULL, CurrentPath, MAX_PATH_SIZE);
	if (GetModuleStatus == NULL) {
		throw error_code::GET_MODULE_FAILED;
	}

	append_gershaim(my_str);
	LSTATUS SetStat = RegSetValueExA(TargetKey, TEXT(AUTORUN_VALUE_NAME), 0, REG_SZ, (BYTE*)my_str, MAX_PATH_SIZE);
	if (SetStat != ERROR_SUCCESS) {
		throw error_code::REGSET_FAILED;
	}

	Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

	BOOL CloseStatus = CloseHandle(g_hMutex);

	if (CloseStatus == NULL) {
		throw error_code::CLOSE_MUTEX_FAILED;
	}

	return error_code::SUCCESS;
	}
	catch (error_code errorCode) {
		return handle_errors(errorCode);
	}
} 











