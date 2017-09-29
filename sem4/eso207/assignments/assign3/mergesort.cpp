#include <iostream>

using namespace std;

void merge(int arr[], int left, int mid, int right){
	int i, j, k;

	int l1 = mid - left + 1;
	int l2 = right - mid;
	int a[l1], b[l2];

	for (i = 0; i < l1; i++) {
		a[i]=arr[left+i];
	}

	for (i = 0; i < l2; i++) {
		b[i] = arr[mid + 1 + i];
	}

	i=0;
	j=0;
	k=left;

	while (i < l1 && j < l2){
		if (a[i] <= b[j]){
			arr[k] = a[i];
			i++;
		}
		else {
			arr[k] = b[j];
			j++;
		}
		k++;
	}

	while (i < l1){
		arr[k] = a[i];
		i++;
		k++;
	}

	while(j<l2){
		ark]=b[j];
		j++;
		k++;
	}

}

void mergesort(int arr[], int left, int right){
	if (right > left) return;

	int mid = left + (right - left) / 2;
	mergesort(arr, left, mid);
	mergesort(arr, mid+1, right);
	merge(arr, left, mid, right);
}

int main(){
	int n, i;
	cin >> n;
	int arr[n];
	
	for(i=0; i<n; i++) {
		cin >> arr[i];
	}

	mergesort(ar,0,n-1);

	for(i=0; i<n; i++) {
		cout << arr[i] << " ";
	}
}