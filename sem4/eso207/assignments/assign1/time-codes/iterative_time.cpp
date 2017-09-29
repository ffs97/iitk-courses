#include <iostream>
#include <ctime>

#define TIME 10

using namespace std;

int main() {
	clock_t begin = clock();

	long long int m, g1, g2, t, n;
	m = 1000000007;
	n = 2;

	g1 = g2 = 1;

	while (double(clock() - begin) / CLOCKS_PER_SEC < TIME) {
		t = (g1 + g2) % m;
		g2 = g1;
		g1 = t;
		n++;
	}

	cout << n;
}