#include <iostream>

using namespace std;

void Op(int arr[], int arr_t[], int len) {
	if (len == 0 || len == 1) return;

	int index = 0;

	for (int i = 2; i < len; i += 3) {
		arr_t[index++] = arr[i];
	}

	for (int i = 1; i < len; i += 3) {
		arr_t[index++] = arr[i];
	}

	for (int i = 0; i < len; i += 3) {
		arr_t[index++] = arr[i];
	}

	for (int i = 0; i < len; i++) {
		arr[i] = arr_t[i];
	}

	Op(arr, arr_t, len/3);
	Op(arr + len/3, len/3 + (len % n > 1));
	Op(arr + 2*(len/3) + (len % n > 1), len/3 + (len % n > 0));
}

int main() {
	int n;
	cin >> n;
	
	long long int sum[n];
	int C[n];
	int C_t[n];

	for (int i = 0; i < n; i++) {
		cin >> C[i];
	}
	Op(C, C_t, n);

	sum[0] = C[0];
	for (int i = 1; i < n; i++) {
		sum[i] = sum[i-1] + C[i];
	}

	int q, start, end, l, r;
	cin >> q;

	while (q--) {
		cin >> start >> end >> l >> r;
		if (start == 0) {
			cout << sum[end];
		}
		else {
			cout << sum[end] - sum[start-1];
		}
		cout << "\n";
	}
}