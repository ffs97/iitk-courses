#include <iostream>
#include <ctime>
#include <cstdio>

typedef long long int ll;

using namespace std;

int main() {
	freopen("iterative.dat", "w", stdout);

	clock_t begin;

	ll m=1000000007, g1, g2, t, n = 1, x;
	int l = 100;

	cout << fixed;
	cout << "#x y\n";

	while (n < 50000000) {
		g1 = g2 = 1;
		x = n;

		begin = clock();

		if (n == 1 || n == 2) {
			g1 = 1;
		}
		else {	
			n -= 2;

			while (n--) {
				t = (g1 + g2) % m;
				g2 = g1;
				g1 = t;
			}
		}
		cout << x << " ";
		cout << double(clock() - begin) / CLOCKS_PER_SEC << "\n";
		n = x + ((x > 500000) ? 1000000 : x);
	}
}