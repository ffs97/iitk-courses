#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

int partition (int arr[], int len) {
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

	return low;
}

void quicksort(int arr[], int len) {
	if (len == 0 || len == 1) return;

	int low = partition(arr, len);

	quicksort(arr, low);
	quicksort(arr+low+1, len-low-1);
}


int main() {
	
	int n;
	cin >> n;
	
	int arr[n];
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}

	quicksort(arr, n);

	for(int i=0; i<n; i++) {
		cout << arr[i] << " ";
	}

}
