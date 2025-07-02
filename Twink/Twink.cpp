#include <iostream>
#include <windows.h>
#include <winuser.h>
#include "TwinkExceptions.h"


const int SECONDS_IN_HOUR = 3600;
const int MILI_MULTIPLIER = 1000;
const int MAX_PATH_SIZE = 260;
const char* AUTORUNS = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
const char* MUTEXNAME = "visual_studio_good";
LPCSTR WELCOME_WINDOW_TITLE = "Management program is up (  it was developed in visual studio happily )";
LPCSTR WELCOME_WINDOW_TEXT = "Visual Studio made me sad";
const char* AUTORUN_VALUE_NAME = "Technician";

HANDLE g_hMutex;
BOOL closeStatus;

enum class StatusCode {
	STATUS_SUCCESS,
	STATUS_ERROR,
};


/*
@brief created a named mutex and makes sure none exist
*/
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

	closeStatus = NULL;
}


/*
@brief pops a message for the user
*/
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


/*
@brief gets the path of the current executable

@return std::string representing the quoted path
*/
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


/*
@brief sets an autorun of the filepath

@param filePath std::string of the executable to be added tothe autorun
*/
void setAutoRun(const std::string& filePath) {
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


/*
@brief Closes the handles if they are still open
*/
void CleanupHandles() {
	if (closeStatus == NULL) {
		CloseHandle(g_hMutex);
	}
}


int main(int argc, char* argv[]) {
	try {
		createSingleMutex();
		userMessageStage();

		std::string filePath = getFilePath();
		setAutoRun(filePath);

		Sleep(SECONDS_IN_HOUR * MILI_MULTIPLIER);

		closeStatus = CloseHandle(g_hMutex);

		if (closeStatus == NULL) {
			throw CloseMutexException();
		}

		return static_cast<int>(StatusCode::STATUS_SUCCESS);
	}
	catch (const MyException& exception) {
		CleanupHandles();
		std::cerr << "An exception occurred (" << exception.getError() << std::endl;
		return static_cast<int>(StatusCode::STATUS_ERROR);
	}
}