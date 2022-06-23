#include <windows.h>
#include <iostream> 
#include <cstdlib>
#include <time.h>
#include <thread>
#include <mutex>

using namespace std;

HANDLE hFull = CreateSemaphore(NULL, 5, 5, L"S2");
mutex forks[5];

class Philosopher {
public:
	int id_;
	mutex& leftFork_;
	mutex& rightFork_;

	Philosopher(int id, mutex& l, mutex& r);
	void think();
	void eat();
	void getLeftFork();
	void getRightFork();
	void putDownForks();
	void run();
};

unsigned long long rand_ms() {
	return rand() % 10;
}

Philosopher::Philosopher(int id, mutex& l, mutex& r) : leftFork_(l), rightFork_(r) {
	id_ = id;
}

void Philosopher::think() {
	cout << id_ << ": thinking... " << endl;
	this_thread::sleep_for(chrono::milliseconds(rand_ms()));
}

void Philosopher::eat() {
	cout << id_ << ": eating... " << endl;
	this_thread::sleep_for(chrono::milliseconds(rand_ms()));
}

void Philosopher::getLeftFork() {
	WaitForSingleObject(hFull, INFINITE);
	leftFork_.lock();
	cout << id_ << ": left fork " << endl;
}

void Philosopher::getRightFork() {
	WaitForSingleObject(hFull, INFINITE);
	rightFork_.lock();
	cout << id_ << ": right fork " << endl;
}

void Philosopher::putDownForks() {
	cout << id_ << ": forks down " << endl;
	ReleaseSemaphore(hFull, 1, NULL);
	rightFork_.unlock();
	ReleaseSemaphore(hFull, 1, NULL);
	leftFork_.unlock();
}

void Philosopher::run() {
	while (true) {
		think();
		getLeftFork();
		getRightFork();
		eat();
		putDownForks();
	}
}

int main() {
	srand(time(NULL));

	Philosopher p1(1, forks[0], forks[1]);
	Philosopher p2(2, forks[1], forks[2]);
	Philosopher p3(3, forks[2], forks[3]);
	Philosopher p4(4, forks[3], forks[4]);
	Philosopher p5(5, forks[4], forks[0]);

	thread t1(&Philosopher::run, &p1);
	thread t2(&Philosopher::run, &p2);
	thread t3(&Philosopher::run, &p3);
	thread t4(&Philosopher::run, &p4);
	thread t5(&Philosopher::run, &p5);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	return 0;
}