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

	pid = getpid();
	msgid = msgget (MSGKEY, 0777);
	printf("<Client %d> got message queue id %d\n", pid, msgid);
	*((int*)msg.buf) = pid;
	msg.type = 1;
	msgsnd(msgid, &msg, sizeof(int), 0);
	printf("<Client %d> sent %d\n", pid, pid);
	msgrcv(msgid, &msg, 64, pid, 0);
	printf("<Client %d> received from pid %d\n", pid, *((int*)msg.buf));
	return 0;
}
