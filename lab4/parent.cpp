#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	int ms = atoi(argv[1]);
	cout << "Process is waiting" << endl;
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"hM");
	HANDLE A = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"A");
	HANDLE B = CreateEvent(NULL, FALSE, FALSE, L"B");
	Sleep(10);
	WaitForSingleObject(hMutex, INFINITE);
	system("cls");

	cout << "Process is working" << endl;

	DWORD dw = WaitForSingleObject(A, INFINITE);
	if (dw == WAIT_OBJECT_0) 
	{
		cout << "Boss message: 1111" << endl;
	}


	for (int i = 0; i < ms; i++) {
		cout << "message\n";
		int input = 0;
		while (input != 2222) {
			cin >> input;
			if (input == 2222) {
				SetEvent(B);
				break;
			}
			if (input != 2222)
				cout << "Wrong input" << endl;
		}
	}

	_cputs("\nPress any key to finish.\n");
	_getch();
	ReleaseMutex(hMutex);
	ExitProcess(1);
	return 0;
}