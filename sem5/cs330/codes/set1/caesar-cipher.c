#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
   int i, j;

   for (i=1; i<argc; i++) {
      j=0;
      while (argv[i][j] != '\0') {
         if ((argv[i][j] >= 'A') && (argv[i][j] <= 'Z')) {
            printf ("%c", 'A'+(argv[i][j]-'A'+23)%26);
         }
         else if ((argv[i][j] >= 'a') && (argv[i][j] <= 'z')) {
            printf ("%c", 'A'+(argv[i][j]-'a'+23)%26);
         }
         else if ((argv[i][j] >= '0') && (argv[i][j] <= '9')) {
            printf ("%c", '0'+(argv[i][j]-'0'+7)%10);
         }
         else printf ("Invalid input.\n");
         j++;
      }
      printf(" ");
   }
   printf("\n");
   return 0;
}
