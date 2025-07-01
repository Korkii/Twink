// Twink.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <winbase.h>


const int MAX_PATH_SIZE = 260;
const char AUTORUNS[46] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
int ShowPopup() {
	int msgboxID = MessageBox(
		NULL,
		(LPCSTR)"Visual Studio made me sad",
		(LPCSTR)"top 3 worst programs to exist",
		MB_ICONINFORMATION
	);
	return msgboxID;
}


int main()
{
	std::cout << "Hello World!\n";
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
	CHAR curr;
	CHAR prev = my_str[0];
	int i;
	for (i = 1; my_str[i - 1] != '\0'; i++) {
		std::cout << i << std::endl;
		curr = my_str[i];
		my_str[i] = prev;
		prev = curr;
	}
	
	my_str[0] = '"';
	my_str[i-1] = '"';
	my_str[i] = '\0';
	char* check = "yes\0";
	RegSetValueExA(TargetKey, TEXT("Technician"), 0, REG_SZ, (BYTE*)my_str, MAX_PATH_SIZE);

} 
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file













