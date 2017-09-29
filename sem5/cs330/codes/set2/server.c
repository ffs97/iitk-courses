#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGKEY 19

typedef struct {
	long type;
	char buf[64];
} msg_t;

int main (void)
{
	int pid, msgid, mypid;
	msg_t msg;

	mypid = getpid();
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
