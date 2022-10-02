#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>

using namespace std;

class bank_account {
	double balance;
	string name;
	mutex m;

public:
	bank_account() {};

	bank_account(double b, string n) :balance(b), name(n) {};

	bank_account(bank_account& const) = delete;

	bank_account& operator=(bank_account& const) = delete;

	void withdraw(double amount) {
		lock_guard<mutex> lg(m);
		balance += amount;
	}

	void deposit(double amount) {
		lock_guard<mutex> lg(m);
		balance += amount;

	}

	void transfer(bank_account& from, bank_account& to, double amount) {
		
		unique_lock<mutex> ul_1(from.m, defer_lock);
		unique_lock<mutex> ul_2(to.m, defer_lock);
		lock(ul_1, ul_2);

		
		from.balance -= amount;
		to.balance += amount;
		cout << "transfer to - " << to.name << " from - " << from.name << " end\n";
	}

};

mutex m1;
mutex m2;

void m1_first_m2_second() {
	lock_guard<mutex> lg1(m1);
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "thread " << this_thread::get_id() << " has acquired lock for m1, its waiting for m2\n";

	lock_guard<mutex> lg2(m2);
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "thread " << this_thread::get_id() << " has acquired lock for m2 mutex \n";

}

void m2_first_m1_second() {
	lock_guard<mutex> lg1(m2);	
	this_thread::sleep_for(chrono::milliseconds(1500));
	cout << "thread " << this_thread::get_id() << " has acquired lock for m2 mutex, its waiting for m1" << endl;

	lock_guard<mutex>lg2(m1);
	cout << "thread " << this_thread::get_id() << " has acquired lock for m1 mutex" << endl;
}

void x_operation() {
	cout << "this is some operation\n";
}

void y_operation() {
	cout << "this is some other operations \n";
}

unique_lock<mutex>get_lock() {
	mutex m;
	unique_lock<mutex> ul(m);
	x_operation();
	return ul;
}

int main() {

	//uncomment whatever you want to execute
	// 
	//run_code_1
	bank_account account;

	bank_account account_1(1000, "James");
	bank_account account_2(2000, "Mathew");

	//thread thread1(&bank_account::transfer, &account, ref(account_1), ref(account_2), 500);
	//this_thread::sleep_for(chrono::milliseconds(100));

	//thread thread2(&bank_account::transfer, &account, ref(account_2), ref(account_1), 200);

	//thread1.join();
	//thread2.join();

	//run_code_2
	//thread thread3(m1_first_m2_second);
	thread thread4(m2_first_m1_second);

	//thread3.join();
	//thread4.join();

	//run_code_3
	unique_lock<mutex> ul(get_lock());
	y_operation();

	return 0;
}
