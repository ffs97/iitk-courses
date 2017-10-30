#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;

#define epsilon 0.000001


bool equals(float a, float b){
	float diff = a-b;
	if (diff <= epsilon && diff >= (-1*epsilon)) {
		return true;
	}
	return false;
}

void generateOutput(int num) {
	printf("%d\n", num);
	pair<float, float> *a = new pair<float, float>[num];
	int i,j;
	for(i = 0; i < num; i++) {
		bool different = false;
		while(!different){
			float f1 = ((float)(rand()%100))/100 + rand()%100;
			float f2 = ((float)(rand()%100))/100 + rand()%100;
			different = true;
			for(j = 0; j < i; j++) {
				if (equals(a[i].first, f1) && equals(a[i].second, f2)) {
					different = false;
					break;
				}
			}
			if (different) {
				a[i] = std::make_pair(f1, f2);
			}
		}
	}
	for(i = 0; i < num; i++) {
		printf("%f %f\n", a[i].first, a[i].second);
	}
}

int main() {
	time_t t;
	srand((unsigned) time(&t));
	int n;
	scanf("%d", &n);
	generateOutput(n);
	return 0;
}
