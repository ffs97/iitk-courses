#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1000000000

int main (void)
{
	int *a, i;
        long long sum=0;

        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));

	for (i=0; i<SIZE; i++) a[i] = i;

        gettimeofday(&tp_start, NULL);
        for (i=0; i<SIZE; i++) sum += a[i];
        gettimeofday(&tp_end, NULL);

        printf("Total sum: %lld\n", sum);
        printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	return 0;
}
