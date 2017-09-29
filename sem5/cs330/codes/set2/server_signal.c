#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <errno.h>

#define MSGKEY 19

typedef struct {
	long type;
	char buf[64];
} msg_t;

int msgid;

void cleanup_and_exit (int signo)
{
	printf("<Server %d> in signal handler for signal %d.\n", getpid(), signo);
	if (msgctl(msgid, IPC_RMID, 0) != 0) {
           printf("Could not delete queue. The errno value is %d\n", errno);
           if (errno == EPERM) {
              printf("You do not seem to have permission to delete the queue.\n");
           }
        }
        else {
           printf("Successfully deleted message queue.\n");
        }
	exit(0);
}

int main (void)
{
	int i, pid, *pidint, mypid;
	msg_t msg;

	for (i=1; i<=19; i++) {
        	signal(i, cleanup_and_exit);
	}

	mypid = getpid();
	msgid = msgget (MSGKEY, 0777);
	printf("<Server %d> got message queue with id %d\n", mypid, msgid);
	msgid = msgget (MSGKEY, 0777 | IPC_CREAT);
	printf("<Server %d> created message queue with id %d\n", mypid, msgid);
	while (1) {
		msgrcv (msgid, &msg, 64, 1, 0);
		pid = *((int*)msg.buf);
		printf("<Server %d> received from pid %d\n", mypid, pid);
		msg.type = pid;
		*((int*)msg.buf) = mypid;
		msgsnd(msgid, &msg, sizeof(int), 0);
		printf("<Server %d> sent %d\n\n", mypid, *((int*)msg.buf));
	}
	return 0;
}
