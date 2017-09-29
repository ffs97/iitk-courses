#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#define SUM_KEY 19
#define SUM_TYPE 1
#define SIZE 1000000000

typedef struct {
	long type;
	char buf[64];
} msg_t;

int main (void)
{
	int *a, i, queue_id;
        msg_t msg;
	unsigned long long child_sum=0, parent_sum=0;
        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));

	for (i=0; i<SIZE; i++) a[i] = i;
	queue_id = msgget(SUM_KEY, 0777 | IPC_CREAT);

        gettimeofday(&tp_start, NULL);

	if (fork() == 0) {
		// This is child
		for (i=0; i<SIZE/2; i++) child_sum += a[i];
		printf("Child saying: my sum is %llu\n", child_sum);
		msg.type = SUM_TYPE;
		*(unsigned long long*)msg.buf = child_sum;
		msgsnd(queue_id, &msg, sizeof(unsigned long long), 0);
        }
        else {
		for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
		printf("Parent saying: my sum is %llu\n", parent_sum);
		printf("Parent saying: <before receive> buffer is %llu\n", *(unsigned long long*)msg.buf);
		msgrcv(queue_id, &msg, sizeof(unsigned long long), SUM_TYPE, 0);
		printf("Parent saying: <after receive> buffer is %llu\n", *(unsigned long long*)msg.buf);
                printf("Total sum: %llu\n", *((unsigned long long*)msg.buf)+parent_sum);
                gettimeofday(&tp_end, NULL);
		wait(NULL);	// Not needed, but a good practice
		msgctl(queue_id, IPC_RMID, NULL);
                printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	}
	return 0;
}
