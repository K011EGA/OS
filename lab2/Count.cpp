#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void fibonachi(long long a, long long b) {
	Sleep(100);
	cout << a << "\n";
	fibonachi(a + b, a);
}

int main() {
	long long b = 0ll;
	long long a = 1ll;
	fibonachi(a, b);
	cin.get();
}