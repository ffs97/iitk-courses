#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

#define CHILD_STACK 16384
#define SIZE 1000000000

/* Contains poor code for waiting on completion of the threads.
*/

int *a;
volatile int doneChild=0, doneParent=0;		// volatile variables are not register-allocated by the compiler
unsigned long long child_sum=0, parent_sum=0;

int add (void *arg)
{
	int i;

        printf("Child saying: My TID: %d, My PID: %d\n", syscall(SYS_gettid), getpid());
	for (i=0; i<SIZE/2; i++) child_sum += a[i];
	printf("Child saying: my sum is %llu\n", child_sum);
        doneChild=1;
        while (!doneParent);
}

int main (void)
{
	int i, child_tid;
	void *child_stack;
        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));
	for (i=0; i<SIZE; i++) a[i] = i;
	child_stack = malloc(CHILD_STACK);

        gettimeofday(&tp_start, NULL);

    	child_tid = clone (add, child_stack+CHILD_STACK, CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, NULL);
        printf("Child TID: %d, Parent TID: %d, Parent PID: %d\n", child_tid, syscall(SYS_gettid), getpid());
	for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
	printf("Parent saying: my sum is %llu\n", parent_sum);
	// wait for child
        while(!doneChild);
        printf("Total sum: %llu\n", child_sum+parent_sum);
        gettimeofday(&tp_end, NULL);
        printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
        doneParent=1;
	return 0;
}
