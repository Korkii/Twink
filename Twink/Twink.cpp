#include <iostream>
#include <windows.h>
#include <winuser.h>
#include "Twink.h"

const int SECONDS_IN_HOUR = 3600;
const int MILI_MULTIPLIER = 1000;
const int MAX_PATH_SIZE = 260;
const char* AUTORUNS = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* MUTEXNAME = "visual_studio_good";
LPCSTR WELCOME_WINDOW_TITLE = "Management program is up (  it was developed in visual studio happily )";
LPCSTR WELCOME_WINDOW_TEXT = "Visual Studio made me sad";
const char* AUTORUN_VALUE_NAME = "Technician";

HANDLE g_hMutex;

AlreadyRunningException::AlreadyRunningException() : MyException{ "Already running" } {};
CreateMutexException::CreateMutexException() : MyException{ "Create Mutex" } {};
CloseMutexException::CloseMutexException() : MyException{ "Close Mutex" } {};
PopupException::PopupException() : MyException{ "Popup" } {};
OpenRegistryException::OpenRegistryException() : MyException{ "Open Registry" } {};
SetRegistryException::SetRegistryException() : MyException{ "Set Registry" } {};
GetModuleException::GetModuleException() : MyException{ "Get Module" } {};

enum class StatusCode {
	STATUS_SUCCESS,
	STATUS_ERROR,
};



void createMutexStage() {
	g_hMutex = OpenMutexA(SYNCHRONIZE, FALSE, MUTEXNAME);
	if (g_hMutex != NULL) {
		throw AlreadyRunningException();
	}
	g_hMutex = CreateMutexA(
		NULL,
		TRUE,
		MUTEXNAME);

	if (g_hMutex == NULL) {
		throw CreateMutexException();
	}
}

void userMessageStage() {
	int msgboxID = MessageBox(
		NULL,
		WELCOME_WINDOW_TITLE,
		WELCOME_WINDOW_TEXT,
		MB_ICONINFORMATION
	);
	if (msgboxID == NULL) {
		throw PopupException();
	}
}

std::string getFilePath() {
	CHAR my_str[MAX_PATH_SIZE];
	LPSTR CurrentPath = my_str;
	DWORD GetModuleStatus = GetModuleFileNameA(NULL, CurrentPath, MAX_PATH_SIZE);
	std::string new_str = my_str;
	new_str = "\"" + new_str + "\"";

	if (GetModuleStatus == NULL) {
		throw GetModuleException();
	}

	return new_str;
}

void SetAutoRun(std::string filePath) {
	HKEY TargetKey;
	LSTATUS OpenStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &TargetKey);
	if (OpenStat != ERROR_SUCCESS) {
		throw OpenRegistryException();
	}

	LSTATUS SetStat = RegSetValueExA(TargetKey, AUTORUN_VALUE_NAME, 0, REG_SZ, (BYTE*)filePath.c_str(), MAX_PATH_SIZE);
	if (SetStat != ERROR_SUCCESS) {
		throw SetRegistryException();
	}
}

int main(int argc, char* argv[]) {
	try {
		createMutexStage();
		userMessageStage();

		std::string filePath = getFilePath();
		SetAutoRun(filePath);

		Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

		BOOL CloseStatus = CloseHandle(g_hMutex);

		if (CloseStatus == NULL) {
			throw CloseMutexException();
		}

		return static_cast<int>(StatusCode::STATUS_SUCCESS);
	}
	catch (MyException exception) {
		std::cerr << "An exception occurred (" << exception.getError() << ")\n";
		return static_cast<int>(StatusCode::STATUS_ERROR);
	}
}