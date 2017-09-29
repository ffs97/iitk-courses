#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1000000000

int main (void)
{
	int *a, i;
        long long child_sum=0, parent_sum=0, sum;
        FILE *fp;

        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));

	for (i=0; i<SIZE; i++) a[i] = i;

        gettimeofday(&tp_start, NULL);

	if (fork() == 0) {
		// This is child
		for (i=0; i<SIZE/2; i++) child_sum += a[i];
		printf("Child saying: my sum is %lld\n", child_sum);
		fp = fopen("child_result.txt", "w");
		fprintf(fp, "%lld\n", child_sum);
		fclose(fp);
        }
        else {
		for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
		printf("Parent saying: my sum is %lld\n", parent_sum);
		// wait for child
		wait (NULL);
		// Read child result
		fp = fopen("child_result.txt", "r");
		fscanf(fp, "%lld", &sum);
		fclose(fp);
                printf("Total sum: %lld\n", sum+parent_sum);
                gettimeofday(&tp_end, NULL);
                printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	}

	return 0;
}
