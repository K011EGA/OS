#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	HANDLE hNamedPipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;
	hNamedPipe = CreateNamedPipe(L"\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		_getch();
		return 0;
	}
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL))
	{
		cerr << "The connection failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	int n;
	cout << "Введите размер массива" << endl;
	cin >> n;
	char* s = new char[n];
	cout << "Введите массив" << endl;
	cin >> s;
	DWORD dwBytesWritten;
	if (!WriteFile(hNamedPipe, &n, sizeof(n), &dwBytesWritten, (LPOVERLAPPED)NULL))
	{
		cerr << "Data writing to the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	for (int i = 0; i < n; i++)
	{
		if (!WriteFile(hNamedPipe, &s[i], sizeof(s), &dwBytesWritten, (LPOVERLAPPED)NULL))
		{
			cerr << "Data writing to the named pipe failed." << endl
				<< "The last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cout << "Press any char to finish the server: ";
			_getch();
			return 0;
		}
	}

	int i = 0;
	cout << "Server отправил : " << n << " " << s << endl;
	int k = 0;
	char* nData = new char[n];
	while (true) {
		k++;
		DWORD dwBytesRead;
		if (!ReadFile(hNamedPipe, &nData[i], sizeof(nData), &dwBytesRead, (LPOVERLAPPED)NULL))
		{
			CloseHandle(hNamedPipe);
			cout << "\nВсе лексемы были получены" << endl;
			_getch();
			return 0;
		}
		cout << "" << nData[i];
		i++;
	}
	cout << endl;
	if (k == 0) {
		cout << "Ничего не было получено" << endl;
	}
	else {
		cout << "Все лексемы были получены" << endl;
	}
	CloseHandle(hNamedPipe);
	cout << "Press any key to exit.\n";
	_getch();
	return 0;
}