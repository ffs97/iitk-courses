#include "syscall.h"

int
main()
{
    int x;
    int sleep_start, sleep_end;

    syscall_wrapper_PrintString("Parent PID: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetPID());
    syscall_wrapper_PrintChar('\n');
    x = syscall_wrapper_Fork();
    if (x == 0) {
       syscall_wrapper_PrintString("Child PID: ");
       syscall_wrapper_PrintInt(syscall_wrapper_GetPID());
       syscall_wrapper_PrintChar('\n');
       syscall_wrapper_PrintString("Child's parent PID: ");
       syscall_wrapper_PrintInt(syscall_wrapper_GetPPID());
       syscall_wrapper_PrintChar('\n');
       sleep_start = syscall_wrapper_GetTime();
       syscall_wrapper_Sleep(100);
       sleep_end = syscall_wrapper_GetTime();
       syscall_wrapper_PrintString("Child called sleep at time: ");
       syscall_wrapper_PrintInt(sleep_start);
       syscall_wrapper_PrintChar('\n');
       syscall_wrapper_PrintString("Child returned from sleep at time: ");
       syscall_wrapper_PrintInt(sleep_end);
       syscall_wrapper_PrintChar('\n');
       syscall_wrapper_PrintString("Child executed ");
       syscall_wrapper_PrintInt(syscall_wrapper_GetNumInstr());
       syscall_wrapper_PrintString(" instructions.\n");
    }
    else {
       syscall_wrapper_PrintString("Parent after fork waiting for child: ");
       syscall_wrapper_PrintInt(x);
       syscall_wrapper_PrintChar('\n');
       syscall_wrapper_Join(x);
       syscall_wrapper_PrintString("Parent executed ");
       syscall_wrapper_PrintInt(syscall_wrapper_GetNumInstr());
       syscall_wrapper_PrintString(" instructions.\n");
    }
    return 0;
}
