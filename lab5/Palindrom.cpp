#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);
	HANDLE R = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"R");
	HANDLE SR = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"SR");
	cout << "В ожидание получения размерности массива" << endl;
	WaitForSingleObject(R, INFINITE);
	int n;
	DWORD dwBytesRead;
	if (!ReadFile(readPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		cout << "Read from the pipe failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return 0;
	}
	char* s = new char[n];
	DWORD dwBytesRead2;
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(readPipe, &s[i], sizeof(s), &dwBytesRead2, NULL))
		{
			cout << "Read from the pipe failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return 0;
		}
	}

	system("cls");
	cout << "Simple получил размер массива: " << n << endl;

	char x;
	cout << "Введите разделитель" << endl;
	cin >> x;
	cout << "Полученный разделитель " << x << endl;

	int k = 0;
	bool key  = true;
	SetEvent(SR);
	for (int i = 0; i < n; i++)
	{
		//cout << s[i] << " ";
		if (s[i] == x)
		{
			for (int j = 0; j < (i - k) / 2; j++)
			{
				if (s[k + j] != s[i - j - 1])
				{
					key = false;
					break;
				}
			}
			//cout << key;
			if (key)
			{
				for (int j = k; j < i; j++)
				{
					cout << s[j];
					DWORD dwBytesWritten;
					if (!WriteFile(writePipe, &s[j], sizeof(s), &dwBytesWritten, NULL))
					{
						cout << "Write to file failed.\n";
						cout << "Press any key to finish.\n";
						_getch();
						return GetLastError();
					}
					SetEvent(SR);
					WaitForSingleObject(R, INFINITE);
				}
				DWORD dwBytesWritten;
				if (!WriteFile(writePipe, &s[i], sizeof(s), &dwBytesWritten, NULL))
				{
					cout << "Write to file failed.\n";
					cout << "Press any key to finish.\n";
					_getch();
					return GetLastError();
				}
				SetEvent(SR);
				WaitForSingleObject(R, INFINITE);

			}
			k = i + 1;
			key = true;
		}

	}
	CloseHandle(writePipe);
	CloseHandle(readPipe);
	cout << "\nВсе найденные полиндромы были отправлены\n";
	cout << "Press any key to exit.\n";
	_getch();
	return 0;
}