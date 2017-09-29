#include <iostream>
#include <ctime>

using namespace std;
typedef long long int ll;

ll temp[2][2], n, m = 1000000000;

ll mt[2][2] = {{1, 0}, {0, 1}};
ll fm[2][2] = {{1, 1}, {1, 0}};

void multiply_matrix(bool t) {
	ll (*arr)[2] = (t) ? mt : fm;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			temp[i][j] = 0;
			for (int k = 0; k < 2; k++) {
				temp[i][j] = (temp[i][j] + (mt[i][k] * arr[k][j]) % m) % m;
			}
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
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

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			mt[i][j] = 0;
		}
		mt[i][i] = 1;
	}

	n -= 2;
	exponent_matrix();

	return (mt[0][0] + mt[0][1]) % m;
}

int main() {
	cin >> n;

	clock_t begin = clock();
	cout << get_G() << " ";
	cout << (double(clock() - begin) / CLOCKS_PER_SEC) << "\n";
}