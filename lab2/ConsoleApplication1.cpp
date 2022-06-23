#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <string>
using namespace std;

int main() {

	setlocale(LC_ALL, "RUS");
	string line = "Child.exe ";

	cout << "Введите кол-во элементов и массив: " << endl;
	int x;
	cin >> x;
	char* mas = new char[x];

	cin.ignore(255,' ');
	cin.getline(mas,x+1);
	
	for (int i = 0; i < x; i++)
		line = line + " " + mas[i];

	wstring c(line.begin(), line.end());
	LPWSTR command_line = &c[0];
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	LPWSTR title = (LPWSTR)L"NAZVANIE";
	si.lpTitle = title;

	boolean check = CreateProcess(NULL, command_line, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp);

	if (!check) {
		_cputs("process not created");
		return 0;
	}
	_cputs("The new process is created.\n");
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	line = "Count.exe";
	wstring str(line.begin(), line.end());
	command_line = &str[0];
	boolean check2 = CreateProcess(NULL, command_line, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp);
	if (check2) 
	{
		cout << "New process has created";
	}
	SetPriorityClass(piApp.hProcess, HIGH_PRIORITY_CLASS);
	Sleep(4000);
	TerminateProcess(piApp.hProcess, 0);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	ExitProcess(0);
}