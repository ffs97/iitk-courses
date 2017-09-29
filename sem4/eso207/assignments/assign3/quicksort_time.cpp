#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

void quicksort(int arr[], int len) {
	if (len == 0 || len == 1) return;

	int low = 1, temp;

	for (int i = 1; i < len; i++) {
		if (arr[i] < arr[0]) {
			temp = arr[i];
			arr[i] = arr[low];
			arr[low++] = temp;
		}
	}
	low = low - 1;

	temp = arr[low];
	arr[low] = arr[0];
	arr[0] = temp;

	quicksort(arr, low);
	quicksort(arr+low+1, len-low-1);
}

int main() {
	int n;
	cin >> n;
	int arr[n];
	for (int i = 0; i < n; i++) {
		arr[i] = i;
	}

	double start, end;

	int k = 1000;
	while (k--) {
		random_shuffle(arr+0, arr+n-1);

		start = clock();
		quicksort(arr, n);
		end = clock();

		cout << (end-start)/CLOCKS_PER_SEC << "\n";
	}
}