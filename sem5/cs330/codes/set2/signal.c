#include <signal.h>

int main (void)
{
   int i, mynum;

   setpgrp();
   printf("parent pid: %d, pgrp: %d\n", getpid(), getpgrp());
	
   for (i=0; i<10; i++) {
      if (fork() == 0) {
         if (i & 1) {
            setpgrp();
	 }
	 printf("[%d] child pid: %d, child pgrp: %d\n", i, getpid(), getpgrp());
	 mynum = i;
         /* The even-numbered children get killed while executing the following
            infinite loop. Each of the odd-numbered children has its own process
            group and remain stuck in the following infinite loop.
         */
	 while (1) {
	    sleep(10);
	    printf("[%d] child pid: %d, child pgrp: %d\n", mynum, getpid(), getpgrp());
	 }
      }
   }
   /* Only the parent process reaches the following statement and sends SIGINT
      to all processes in its process group (i.e., the even-numbered children
      and itself).
   */
   kill (0, SIGINT);
   printf("Killed all in group %d\n", getpgrp()); /* Never printed */
   return 0;
}
