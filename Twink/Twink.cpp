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



void createSingleMutex() {
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
	CHAR myPath[MAX_PATH_SIZE];
	LPSTR currentPath = myPath;
	DWORD getModuleStatus = GetModuleFileNameA(NULL, currentPath, MAX_PATH_SIZE);
	std::string newPath = myPath;
	newPath = "\"" + newPath + "\"";

	if (getModuleStatus == NULL) {
		throw GetModuleException();
	}

	return newPath;
}

void SetAutoRun(std::string filePath) {
	HKEY targetKey;
	LSTATUS openStat = RegOpenKeyA(HKEY_CURRENT_USER, AUTORUNS, &targetKey);
	if (openStat != ERROR_SUCCESS) {
		throw OpenRegistryException();
	}

	LSTATUS setStat = RegSetValueExA(targetKey, AUTORUN_VALUE_NAME, 0, REG_SZ, reinterpret_cast<const BYTE*>(filePath.c_str()), MAX_PATH_SIZE);
	if (setStat != ERROR_SUCCESS) {
		throw SetRegistryException();
	}
}

int main(int argc, char* argv[]) {
	try {
		createSingleMutex();
		userMessageStage();

		std::string filePath = getFilePath();
		SetAutoRun(filePath);

		Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

		BOOL closeStatus = CloseHandle(g_hMutex);

		if (closeStatus == NULL) {
			throw CloseMutexException();
		}

		return static_cast<int>(StatusCode::STATUS_SUCCESS);
	}
	catch (MyException exception) {
		std::cerr << "An exception occurred (" << exception.getError() << std::endl;
		return static_cast<int>(StatusCode::STATUS_ERROR);
	}
}