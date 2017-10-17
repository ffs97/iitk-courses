#include "syscall.h"
#define SIZE 100
int
main()
{
    int array[SIZE], i, sum=0;

    syscall_wrapper_PrintString("Starting physical address of array: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetPA((unsigned)array));
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Physical address of array[50]: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetPA(&array[50]));
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Current physical address of stack top: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetPA(syscall_wrapper_GetReg(29)));
    syscall_wrapper_PrintChar('\n');
    for (i=0; i<SIZE; i++) array[i] = i;
    syscall_wrapper_PrintString("We are currently at PC: ");
    syscall_wrapper_PrintIntHex(syscall_wrapper_GetReg(34));
    syscall_wrapper_PrintChar('\n');
    for (i=0; i<SIZE; i++) sum += array[i];
    syscall_wrapper_PrintString("Total sum: ");
    syscall_wrapper_PrintInt(sum);
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_Exit(0);
    return 0;
}
