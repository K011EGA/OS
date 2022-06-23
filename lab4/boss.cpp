#include <iostream>
#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#pragma warning(disable : 4996)
using namespace std;
int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"hM");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	HANDLE A = CreateEvent(NULL, FALSE, FALSE, L"A");


	setlocale(LC_ALL, "rus");
	TCHAR lpszCommandLine1[256] = L"Parent.exe";
	TCHAR lpszCommandLine2[256] = L"Child.exe";
	TCHAR lpszCommandLine3[256] = L"Child.exe";
	TCHAR lpszCommandLine4[256] = L"Parent.exe";
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	int ps, cs, ms;
	cout << "Количество процессоров Parent" << endl;
	cin >> ps;
	cout << "Количество процессоров  Child" << endl;
	cin >> cs;
	cout << "Количество сообщений, принятых от Parent или Child" << endl;
	cin >> ms;
	HANDLE semaphore = CreateSemaphore(NULL, 3, 3, L"S");
	PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[ps + cs];
	HANDLE* piH = new HANDLE[ps + cs];
	int pi = 0;
	TCHAR buf[300];
	_stprintf(buf, TEXT(" %d"), ms/ps);
	_tcscat(lpszCommandLine1, buf);
	_stprintf(buf, TEXT(" %d"), ms/cs);
	_tcscat(lpszCommandLine2, buf);
	for (int i = 0; i < ps-1; i++) {
		if (!CreateProcess(NULL, lpszCommandLine1, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}

	_stprintf(buf, TEXT(" %d"), (ms / ps + ms % ps));
	_tcscat(lpszCommandLine4, buf);
	if (!CreateProcess(NULL, lpszCommandLine4, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	piH[pi] = piApp[pi].hProcess;
	pi++;

	for (int i = 0; i < cs-1; i++) {
		if (!CreateProcess(NULL, lpszCommandLine2, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}

	_stprintf(buf, TEXT(" %d"),(ms / cs + ms % cs));
	_tcscat(lpszCommandLine3, buf);
	if (!CreateProcess(NULL, lpszCommandLine3, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}



	for (int i = 0; i < ps; i++)
	{
		cout << "message\n";
		int input = 0;
		while (input != 1111) {
			cin >> input;
			if (input == 1111) {
				SetEvent(A);
				break;
			}
			if (input != 1111)
				cout << "Wrong input" << endl;
		}
	}


	WaitForMultipleObjects(ps + cs, piH, TRUE, INFINITE);
	for (int i = 0; i < ps + cs; i++) {
		CloseHandle(piApp[i].hThread);
		CloseHandle(piApp[i].hProcess);
	}
	CloseHandle(hMutex);
	CloseHandle(semaphore);
	return 0;
}