#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>

#define CHILD_STACK 16384
#define SIZE 1000000000

int *a;
unsigned long long child_sum=0, parent_sum=0;

int add (void *arg)
{
	int i;

	for (i=0; i<SIZE/2; i++) child_sum += a[i];
	printf("Child saying: my sum is %llu\n", child_sum);
}

int main (void)
{
	int i;
	void *child_stack;
        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));
	for (i=0; i<SIZE; i++) a[i] = i;
	
        child_stack = malloc(CHILD_STACK);

        gettimeofday(&tp_start, NULL);

    	clone (add, child_stack+CHILD_STACK, SIGCHLD | CLONE_VM, NULL);
	for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
	printf("Parent saying: my sum is %llu\n", parent_sum);
	// wait for child
	wait (NULL);
        printf("Total sum: %llu\n", child_sum+parent_sum);
        gettimeofday(&tp_end, NULL);
        printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	return 0;
}
