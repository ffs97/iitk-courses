#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 1000000000

int *a, num_threads;
unsigned long long *private_sum;

void *solver (void *param)
{
	int i, id = *(int*)(param), j;
	private_sum[id] = 0;
	
        if (num_threads == 2) {
           for (i=SIZE/2; i<SIZE; i++) {
                private_sum[id] += a[i];
           }
        }
        else {
           j = (SIZE/num_threads)*(id+1);
	   for (i=(SIZE/num_threads)*id; i<j; i++) {
		private_sum[id] += a[i];
	   }
        }
}	

int main (int argc, char *argv[])
{
	int i, j, *tid;
	pthread_t *threads;
	pthread_attr_t attr;
        struct timeval tp_start, tp_end;
        unsigned long long sum=0; 

	if (argc != 2) {
		printf ("Need number of threads.\n");
		exit(1);
	}
	num_threads = atoi(argv[1]);
	threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
	private_sum = (unsigned long long*)malloc(num_threads*sizeof(unsigned long long));
        tid = (int*)malloc(num_threads*sizeof(int));
        for (i=0; i<num_threads; i++) tid[i] = i;

        a = (int*)malloc(SIZE*sizeof(int));
	for (i=0; i<SIZE; i++) a[i] = i;

	pthread_attr_init(&attr);

        gettimeofday(&tp_start, NULL);

	for (i=1; i<num_threads; i++) {
                /* pthread_create arguments: thread pointer,
                                             attribute pointer,
                                             function pointer,
                                             argument pointer to the function
                */
		pthread_create(&threads[i], &attr, solver, &tid[i]);
   	}
        if (num_threads == 1) {
           for (i=0; i<SIZE; i++) {
                sum += a[i];
           }
        }
        else if (num_threads == 2) {
           for (i=0; i<SIZE/2; i++) {
                sum += a[i];
           }
        }
        else {
           j=SIZE/num_threads;
	   for (i=0; i<j; i++) {
		sum += a[i];
	   }
        }
	
	for (i=1; i<num_threads; i++) {
		pthread_join(threads[i], NULL);
		sum += private_sum[i];
	}

	printf("SUM: %llu\n", sum);

        gettimeofday(&tp_end, NULL);
        printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));

	return 0;
}
