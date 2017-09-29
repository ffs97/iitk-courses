#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

#define SUM_KEY 19

#define SIZE 1000000000

int main (void)
{
	int *a, i, shm_id;
	unsigned long long *shm;
	unsigned long long child_sum=0, parent_sum=0;
        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));

	for (i=0; i<SIZE; i++) a[i] = i;
	shm_id = shmget(SUM_KEY, 4096, 0777 | IPC_CREAT);

        gettimeofday(&tp_start, NULL);
 
	if (fork() == 0) {
		// This is child
		for (i=0; i<SIZE/2; i++) child_sum += a[i];
		printf("Child saying: my sum is %llu\n", child_sum);
		shm = (unsigned long long*)shmat(shm_id, 0, 0);
		shm[10] = child_sum;		// Could use any other index in shm as well.
						// In this case, shm holds 4096 bytes i.e., 512 unsigned long long.
						// So, any index in the range 0 to 511 would be fine.
        }
        else {
		for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
		printf("Parent saying: my sum is %llu\n", parent_sum);
		shm = (unsigned long long*)shmat(shm_id, 0, 0);
		wait(NULL);
                printf("Total sum: %llu\n", shm[10]+parent_sum);
                gettimeofday(&tp_end, NULL);
		shmctl(shm_id, IPC_RMID, NULL);
                printf("Total time: %lld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	}
	shmdt(shm);
	return 0;
}
