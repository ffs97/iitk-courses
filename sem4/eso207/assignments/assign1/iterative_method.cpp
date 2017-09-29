#include <iostream>
#include <ctime>

typedef long long int ll;

using namespace std;

int main() {
	int T;
	cin >> T;

	ll a, b, c, m, g1, g2, t, n;

	while (T--) {
		cin >> a >> b >> c;
		cin >> n >> m;

		a %= m;
		b %= m;
		c %= m;

		g1 = g2 = 1;

		if (n == 1 || n == 2) {
			cout << 1 << "\n";
			continue;
		}
		n -= 2;

		while (n--) {
			t = ((a*g1 % m) + (b*g2 % m) + c) % m;
			g2 = g1;
			g1 = t;
		}

		cout << g1 << "\n";
	}
}