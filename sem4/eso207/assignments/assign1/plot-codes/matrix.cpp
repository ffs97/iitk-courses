#include <iostream>
#include <ctime>

using namespace std;

long int m = ;

long int get_G(int n) {
	if (n == 1 || n == 2) return 1;
	return (get_G(n-1) + get_G(n-2)) % m;
}

int main() {
	clock_t begin;
	int n = 1;
	while (++n < 50) {
		begin = clock();
		get_G(n)
		cout << n << " " << log();
	}
}