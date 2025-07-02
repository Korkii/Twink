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

enum class StatusCode {
	SUCCESS,
	ALREADY_OPEN,
	CREATE_MUTEX_FAILED,
	CLOSE_MUTEX_FAILED,
	POPUP_FAILED,
	REGOPEN_FAILED,
	REGSET_FAILED,
	GET_MODULE_FAILED,
};



/*
@brief Legit appends gershaim.

@param my_str The null terminated string to append garshiaim to
*/
void appendQuotes(CHAR* my_str) {
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
@brief Handles all error types

@param errorCode Error code to take care of
@return errorcode
*/
int handleErrors(StatusCode errorCode) {
	switch (errorCode) {
	case StatusCode::ALREADY_OPEN:
		std::cerr << "Already open" << std::endl;
		return static_cast<int>(errorCode);
	default:
		std::cerr << GetLastError() << std::endl;
		return static_cast<int>(errorCode);
	}
}


int main(int argc, char* argv[])
{
	try {
		g_hMutex = OpenMutexA(SYNCHRONIZE, FALSE, MUTEXNAME);
		if (g_hMutex != NULL) {
			throw StatusCode::ALREADY_OPEN;
		}
		g_hMutex = CreateMutexA(
			NULL,
			TRUE,
			MUTEXNAME);

		if (g_hMutex == NULL) {
			throw StatusCode::CREATE_MUTEX_FAILED;
		}

		int msgboxID = MessageBox(
			NULL,
			WELCOME_WINDOW_TITLE,
			WELCOME_WINDOW_TEXT,
			MB_ICONINFORMATION
		);
		if (msgboxID == NULL) {
			throw StatusCode::POPUP_FAILED;
		}

		HKEY TargetKey;
		LSTATUS OpenStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &TargetKey);
		if (OpenStat != ERROR_SUCCESS) {
			throw StatusCode::REGOPEN_FAILED;
		}

		CHAR my_str[MAX_PATH_SIZE];
		LPSTR CurrentPath = my_str;
		// std::string to LPSTR
		DWORD GetModuleStatus = GetModuleFileNameA(NULL, CurrentPath, MAX_PATH_SIZE);
		std::string new_str = my_str;
		new_str = "\"" + new_str + "\"";

		if (GetModuleStatus == NULL) {
			throw StatusCode::GET_MODULE_FAILED;
		}
		
		
		LSTATUS SetStat = RegSetValueExA(TargetKey, AUTORUN_VALUE_NAME, 0, REG_SZ, (BYTE*)new_str.c_str(), MAX_PATH_SIZE);
		if (SetStat != ERROR_SUCCESS) {
			throw StatusCode::REGSET_FAILED;
		}

		Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

		BOOL CloseStatus = CloseHandle(g_hMutex);

		if (CloseStatus == NULL) {
			throw StatusCode::CLOSE_MUTEX_FAILED;
		}

		return static_cast<int>(StatusCode::SUCCESS);
	}
	catch (StatusCode errorCode) {
		return handleErrors(errorCode);
	}
}