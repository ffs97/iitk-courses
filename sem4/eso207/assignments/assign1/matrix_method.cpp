#include <iostream>

using namespace std;
typedef long long int ll;

ll temp[3][3], n, a, b, c, m;

ll mt[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
ll fm[3][3] = {{0, 0, 0}, {1, 0, 0}, {0, 0, 1}};

void multiply_matrix(bool t) {
	ll (*arr)[3] = (t) ? mt : fm;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				temp[i][j] = (temp[i][j] + (mt[i][k] * arr[k][j]) % m) % m;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mt[i][j] = temp[i][j];
		}
	}
}

void exponent_matrix() {
	if (n == 0) return;
	if (n % 2 == 0) {
		n /= 2;
		exponent_matrix();
		multiply_matrix(true);
	}
	else {
		n -= 1;
		exponent_matrix();
		multiply_matrix(false);
	}
}

long int get_G() {
	if (n == 1 || n == 2) return 1;
	
	fm[0][0] = a;
	fm[0][1] = b;
	fm[0][2] = c;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mt[i][j] = 0;
		}
		mt[i][i] = 1;
	}

	n -= 2;
	exponent_matrix();

	return (mt[0][0] + mt[0][1] + mt[0][2]) % m;
}

int main() {
	int T;
	cin >> T;

	while (T--) {
		cin >> a >> b >> c;
		cin >> n >> m;

		a %= m;
		b %= m;
		c %= m;

		cout << get_G() << "\n";
	}
}