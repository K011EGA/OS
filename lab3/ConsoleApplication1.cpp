#include <Windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
#include <string>
using namespace std;

HANDLE work, SumElement;
DWORD IDwork, IDs;
CRITICAL_SECTION cs;
CRITICAL_SECTION cs2;
CRITICAL_SECTION cs3;
HANDLE hOutEvent;
double mean;
int length;
int k;
int* arr;
char* temp;
int vremya;

DWORD WINAPI Work(LPVOID)
{
	EnterCriticalSection(&cs);
	cout << "введите количество секунд паузы ";
	cin >> vremya;
	vremya *= 1000;
	int count = 0;

	for (int i = 0; i < length; i++) {
		if (arr[i] <= 9) {
			string s = to_string(arr[i]);
			const char* nchar = s.c_str();
			temp[i] = *nchar;
			cout << arr[i];
			Sleep(vremya);
			if (i - count == k - 1) 
			{
				//LeaveCriticalSection(&cs2);
			}
		}
		else {
			count++;
			temp[i] = ' ';
		}
	}
	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI Sum(LPVOID) {
	EnterCriticalSection(&cs2);
	double sum = 0;
	double count = 0;
	int i = 0, m = 0;
	while (i - m < k) {
		if (temp[i] != ' ') {
			sum += atoi((&temp[i]));
			count++;
		}
		else {
			m++;
		}
		i++;
	}
	mean = sum / count;
	SetEvent(hOutEvent);
	LeaveCriticalSection(&cs2);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "RUS");

	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);
	InitializeCriticalSection(&cs3);
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();
	//LeaveCriticalSection(&cs2);
	EnterCriticalSection(&cs2);
	work = CreateThread(NULL, 0, Work, NULL, CREATE_SUSPENDED, &IDwork);
	if (work == NULL) return GetLastError();
	SumElement = CreateThread(NULL, 0, Sum, NULL, CREATE_SUSPENDED, &IDs);
	if (SumElement == NULL) return GetLastError();


	cout << "введите размер массива ";
	cin >> length;
	arr = new int[length];
	temp = new char[length];
	cout << "введите массив ";
	for (int i = 0; i < length; i++) {
		cin >> arr[i];
	}


	cout << "введите k ";
	cin >> k;
	//EnterCriticalSection(&cs2);
	ResumeThread(work);
	ResumeThread(SumElement);
	Sleep(5);
	EnterCriticalSection(&cs);
	
	cout << endl << "итговый массив " << endl;
	for (int i = 0; i < length; i++)
	{
		cout << temp[i];
	}
	LeaveCriticalSection(&cs2);
		
	//EnterCriticalSection(&cs2);

	WaitForSingleObject(hOutEvent, INFINITE);

	cout << endl << "среднее арифметическое первых " << k << " элементов \n";
	cout << mean;

	return 0;
}