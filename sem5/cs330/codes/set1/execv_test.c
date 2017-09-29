#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	printf("The game is never over, John. But there may be some new players now.\n");
	printf("Pid before execv: %d\n", getpid());
	execv("print",NULL);
	printf("Returned from execv call.\n");
	return 0;
}
