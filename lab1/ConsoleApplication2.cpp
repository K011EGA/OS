#include <windows.h>
#include <process.h>
#include <iostream>
#include <time.h>

using namespace std;

UINT WINAPI threadWorker(LPVOID mass2)
{
	int* mass = (int*)mass2;
	int min, k = 0;
	for (int i = 1; i < mass[0] + 1; i++)
	{
		if (mass[i] > 0)
		{
			min = mass[i];
			k = 1;
			break;
		}
	}

	cout << "На сколько секунд вы хотите приостановить поток?" << endl;
	int sec;
	cin >> sec;
	Sleep(sec * 1000);
	cout << "Поток Worker возобновил свою работу." << endl;

	if (!k)
	{
		cout << "нет положительных элементов массива\n";
		return 0;
	}

	for (int i = 1; i < mass[0] + 1; i++)
	{
		if (mass[i] < min && mass[i] > 0)
			min = mass[i];
	}
	cout << "минимальный элемент из положительных элементов массива " << min << endl;

	return 0;
}

DWORD WINAPI threadMain(int n)
{
	int* mass = new int[n + 1];
	mass[0] = n;
	for (int i = 1; i < n + 1; i++)
	{
		mass[i] = rand() % 20 - 10;
		cout << mass[i] << " ";
	}
	cout << "\n";

	LPVOID lpData;
	lpData = (LPVOID)mass;

	HANDLE	hThreadWorker;
	UINT	IDThreadWorker;


	hThreadWorker = (HANDLE)_beginthreadex(NULL, 0, threadWorker, lpData, 0, &IDThreadWorker);
	SuspendThread(hThreadWorker);
	cout << "Через сколько секунд вы хотите начать работу потока Worker?" << endl;
	int sec;
	cin >> sec;
	Sleep(sec * 1000);
	ResumeThread(hThreadWorker);


	WaitForSingleObject(hThreadWorker, INFINITE);
	CloseHandle(hThreadWorker);

	return 0;
}
DWORD WINAPI Count()
{
	int* mass = new int[10];
	mass[0] = 0;
	cout << mass[0] << " ";
	mass[1] = 1;
	cout << mass[1] << " ";
	for (int i = 2; 1; i++)
	{
		mass[i] = mass[i - 1] + mass[i - 2];
		cout << mass[i] << " ";
	}
	cout << "\n";

	return 0;
}
int main()
{
	setlocale(LC_ALL, "Russian");
	int n;
	HANDLE hThread;
	DWORD IDThread;
	srand(time(NULL));
	cout << "Введите размерность массива ";
	cin >> n;

	LPVOID lpData;
	lpData = (LPVOID)n;
	// можно на структуру

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadMain, lpData, 0, &IDThread);

	//hThread = (HANDLE)_beginthreadex(NULL, 0, thread, &hEvent, 0, &IDThread);

	if (hThread == NULL)
		return GetLastError();

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);



	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Count, 0, CREATE_SUSPENDED, &IDThread);

	cout << "Запустить count 1" << endl;
	cin >> n;
	if (n)
		ResumeThread(hThread);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}