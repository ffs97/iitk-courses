#include <iostream>
#include <ctime>

using namespace std;

long int m = 1000000000;

long int get_G(long long int n) {
	if (n == 1 || n == 2) return 1;
	return (get_G(n-1) + get_G(n-2)) % m;
}

int main() {
	long long int n = 1;

	double maxtime;
	cin >> maxtime;

	clock_t begin;
	while (true) {
		begin = clock();
		get_G(n);
		if (double(clock() - begin) / CLOCKS_PER_SEC > maxtime) {
			cout << "MAX VALUE: " << n << "\n";
			break;
		}
		n++;
	}
}