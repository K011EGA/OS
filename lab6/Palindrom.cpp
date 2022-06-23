#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	HANDLE hNamedPipe;
	char machineName[80];
	TCHAR pipeName[80];
	DWORD dwBytesWritten;
	cout << "Enter a name of the server machine: ";
	cin >> machineName;
	wsprintf(pipeName, L" ", machineName);


	hNamedPipe = CreateFile(L"\\\\.\\pipe\\demo_pipe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		_getch();
		return 0;
	}
	cout << "В ожидание получения размерности массива" << endl;
	int n;
	DWORD dwBytesRead;
	if (!ReadFile(hNamedPipe, &n, sizeof(n), &dwBytesRead, (LPOVERLAPPED)NULL))
	{
		cerr << "Data reading from the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	char* s = new char[n];
	DWORD dwBytesRead2;
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(hNamedPipe, &s[i], sizeof(s), &dwBytesRead2, (LPOVERLAPPED)NULL))
		{
			cerr << "Data reading from the named pipe failed." << endl
				<< "The last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cout << "Press any char to finish the server: ";
			_getch();
			return 0;
		}
	}

	system("cls");
	cout << "Polindrom получил размер массива: " << n << endl;

	char x;
	cout << "Введите разделитель" << endl;
	cin >> x;
	cout << "Полученный разделитель " << x << endl;

	int k = 0;
	bool key = true;
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
					cout << s[j] << endl;
					DWORD dwBytesWritten;
					if (!WriteFile(hNamedPipe, &s[j], sizeof(s), &dwBytesWritten, (LPOVERLAPPED)NULL))
					{
						cerr << "Writing to the named pipe failed: " << endl
							<< "The last error code: " << GetLastError() << endl;
						cout << "Press any char to finish the client: ";
						_getch();
						CloseHandle(hNamedPipe);
						return 0;
					}
				}
				DWORD dwBytesWritten;
				if (!WriteFile(hNamedPipe, &s[i], sizeof(s), &dwBytesWritten, (LPOVERLAPPED)NULL))
				{
					cerr << "Writing to the named pipe failed: " << endl
						<< "The last error code: " << GetLastError() << endl;
					cout << "Press any char to finish the client: ";
					_getch();
					CloseHandle(hNamedPipe);
					return 0;
				}


			}
			k = i + 1;
			key = true;
		}

	}
	CloseHandle(hNamedPipe);
	cout << "\nВсе найденные полиндромы были отправлены\n";
	cout << "Press any key to exit.\n";
	_getch();
	return 0;
}