#include "syscall.h"
#define SIZE 100
int
main()
{
    int array[SIZE], i, sum=0;

    for (i=0; i<SIZE; i++) array[i] = i;
    for (i=0; i<SIZE; i++) sum += array[i];
    syscall_wrapper_PrintString("Total sum: ");
    syscall_wrapper_PrintInt(sum);
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Executed instruction count: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetNumInstr());
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_Exit(0);
    return 0;
}
