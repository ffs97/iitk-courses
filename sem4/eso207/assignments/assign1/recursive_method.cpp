#include <iostream>
#include <ctime>

using namespace std;

long int a, b, c, m;

long int get_G(long long int n) {
	if (n == 1 || n == 2) return 1;
	return ((a * get_G(n-1)) %m + (b * get_G(n-2)) % m + c) % m;
}

int main() {
	int T;
	cin >> T;

	long long int n;
	while (T--) {
		cin >> a >> b >> c;
		cin >> n >> m;

		a %= m;
		b %= m;
		c %= m;

		cout << get_G(n) << "\n";
	}
}