#include <iostream>
#include <cmath>

using namespace std;

long long int n, l, r, i, j, mod_value = 1000000007;

long long int compute (long long int a, long long int s, long long int d, long long int start, long long int end) {
	if (i > j || start > end || a > r || s < l || a > s || i > end || j < start) return 0;

	if (i <= start && j >= end) {
		a = a + max((long long int) 0, (long long int) ceil((l - a) * 1.0 / d)) * d;
		s = a + floor((r - a) * 1.0 / d) * d;
		
		long long int len = ((s - a) / d + 1);

		if (len % 2 == 0) {
			return (((s + a) % mod_value) * ((len / 2) % mod_value)) % mod_value;
		}
		else {
			return ((((s + a) / 2) % mod_value) * (len % mod_value)) % mod_value;
		}
	}
	else {
		long long int sum = 0;
		long long int len, len_t = end - start + 1;

		len = len_t / 3;
		if (len) {
			sum = (sum + compute(a + 2*d, a + (3*len - 1) * d, 3*d, start, start + len - 1)) % mod_value;
		}
		start += len;

		len_t -= len;
		len = len_t / 2;
		if (len) {
			sum = (sum + compute(a + d, a + (3*len - 2) * d, 3*d, start, start + len - 1)) % mod_value;
		}
		start += len;

		len_t -= len;
		len = len_t;
		if (len) {
			sum = (sum + compute(a, a + (3*len - 3) * d, 3*d, start, start + len - 1)) % mod_value;
		}

		return sum;
	}
}

int main() {
	cin >> n;

	int q;
	cin >> q;

	while (q--) {
		cin >> i >> j >> l >> r;
		cout << compute(0, n-1, 1, 0, n-1) << "\n";
	}

	return 0;
}
