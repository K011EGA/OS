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
	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"S");
	HANDLE B = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"B");
	cout << "Process is waiting" << endl;
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");

	cout << "Process is working" << endl;

	for (int i = 0; i < ms; i++) {
		DWORD dw = WaitForSingleObject(B, INFINITE);
		if (dw == WAIT_OBJECT_0)
		{
			cout << "Parent message: 2222" << endl;
		}
	}
    ReleaseSemaphore(semaphore, 1, NULL);
	_cputs("\nPress any key to finish.\n");
	_getch();
	
	ExitProcess(1);
	return 0;
}