#include "syscall.h"

#define OUTER_BOUND 30
#define SIZE 100

int
main() {
    int array[SIZE], i, k, sum, pid = syscall_wrapper_GetPID() - 1;
    unsigned start_time, end_time;

    start_time = syscall_wrapper_GetTime();
    for (k = 0; k < OUTER_BOUND; k++)
    {
        for (i = 0; i < SIZE; i++) sum += array[i];
        syscall_wrapper_PrintInt(pid);
        syscall_wrapper_PrintInt(pid);
    }
    end_time = syscall_wrapper_GetTime();
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Total sum: ");
    syscall_wrapper_PrintInt(sum);
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Start time: ");
    syscall_wrapper_PrintInt(start_time);
    syscall_wrapper_PrintString(", End time: ");
    syscall_wrapper_PrintInt(end_time);
    syscall_wrapper_PrintString(", Total time: ");
    syscall_wrapper_PrintInt(end_time - start_time);
    syscall_wrapper_PrintChar('\n');
    return 0;
}
