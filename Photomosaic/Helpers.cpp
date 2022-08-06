#include "Helpers.h"

string receaveUserInput() {
	string receaving;
	cin >> receaving;
	return receaving;
}

void Counter::resetCount() {
	count = 0;
}

void Counter::setTotal(size_t t) {
	total = t;
	//progressBar1->Maximum = t;
}

void Counter::printCount() {
	cout << 100 * count / total << "% done" << '\r' << flush;
	count++;
}