#pragma once
#include <string>
#include <iostream>

using namespace std;

string receaveUserInput();



struct Counter {
	static size_t count;
	static size_t total;

	static void resetCount();
	static void setTotal(size_t t);
	static void printCount();


};