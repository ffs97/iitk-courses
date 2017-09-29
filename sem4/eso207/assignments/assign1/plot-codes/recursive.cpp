#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdio>

using namespace std;

long long int m = 1000000007;

long int get_G(int n) {
	if (n == 1 || n == 2) return 1;
	return (get_G(n-1) + get_G(n-2)) % m;
}

int main() {
	freopen("recursive.dat", "w", stdout);

	clock_t begin;
	int n = 2;
	while (++n < 40) {
		begin = clock();
		get_G(n);
		cout << n << " ";
		cout << log(((double) (clock() - begin) / CLOCKS_PER_SEC)) << "\n";
	}
}