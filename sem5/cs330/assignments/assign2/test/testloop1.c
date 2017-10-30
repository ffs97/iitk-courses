#include "syscall.h"


#define OUTER_BOUND 4
#define SIZE 100


int
main() {
    int array[SIZE], i, k, sum, pid = syscall_wrapper_GetPID() - 1;
    unsigned start_time, end_time;

    for (k = 0; k < OUTER_BOUND; k++) {
        for (i = 0; i < SIZE; i++) sum += array[i];
        syscall_wrapper_PrintInt(pid);
        syscall_wrapper_PrintInt(pid);
    }
    syscall_wrapper_PrintChar('\n');
    syscall_wrapper_PrintString("Total sum: ");
    syscall_wrapper_PrintInt(sum);
    syscall_wrapper_PrintChar('\n');
    return 0;
}
