#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
   char commandline[100], **argv;
   int i, j, k, n;
   while (1) {
      printf("> ");     // Prompt for a command
      j=0;
      i=0;
      k=0;
      gets(commandline);     // Read command line
      if (!strcmp(commandline, "exit")) break;

      // Calculate the number of command line arguments
      while (commandline[i] != '\0') {
         while(commandline[i] == ' ') i++;
         i++;
         if (commandline[i] == ' ') { j++; i++; }
      }

      // Allocate argument list array (one plus the needed number)
      argv = (char**)malloc((j+2)*sizeof(char*));
      for (n=0; n<j+1; n++) {
         argv[n] = (char*)malloc(20*sizeof(char));
      }
      argv[j+1] = NULL;   // The last argument is null

      // Copy command line to argument list
      j=0;
      i=0;
      k=0;
      while (commandline[i] != '\0') {
         while(commandline[i] == ' ') i++;
         argv[j][k] = commandline[i];
         k++;
         i++;
         if (commandline[i] == ' ') {
            argv[j][k] = '\0';
            j++;
            k=0;
            i++;
         }
      }
      argv[j][k] = '\0';

      // Fork child to execute command
      if (fork()==0) execv(argv[0],argv);
      else wait(NULL);
   }
   return 0;
}
