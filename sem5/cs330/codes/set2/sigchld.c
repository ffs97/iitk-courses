#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

int main (int argc, char *argv[])
{
	int i, pid;

	signal (SIGINT, SIG_IGN);

	if (argc > 1) signal(SIGCHLD, SIG_IGN);

	printf("This is parent with pid %d\n", getpid());

	for (i=0; i<5; i++) {
		if (fork() == 0) {
			printf("Child[%d] has pid %d\n", i, getpid());
			if (i%2 == 0) while(1);
			else break;
		}
	}

	pid = wait(NULL);
	printf("Process[%d]: Child with pid %d has terminated\n", getpid(), pid);
	return 0;
}
