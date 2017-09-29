#include<setjmp.h>

main()
{
	jmp_buf env;
	int i;
	
	i=setjmp(env);
	printf("i= %d\n",i);
	if(i==0)
		printf("I am in if ..\n");
	else{
		printf("I am in else too...\n");
		exit(0);
	}
	longjmp(env,2);
	printf("Grrr... why am i not getting printed\n");
}
