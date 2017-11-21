#include "syscall.h"

int
main() {
    int x, y = 6;
    int sleep_start, sleep_end;

    syscall_wrapper_PrintString("Parent PID: ");
    syscall_wrapper_PrintInt(syscall_wrapper_GetPID());
    syscall_wrapper_PrintChar('\n');
    x = syscall_wrapper_Fork();
    if (x == 0)
    {
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
        y++;
        syscall_wrapper_PrintString("Child y=");
        syscall_wrapper_PrintInt(y);
        syscall_wrapper_PrintChar('\n');
        x = syscall_wrapper_Fork();
        syscall_wrapper_Exec("../test/printtest");
        if (x == 0)
        {
            syscall_wrapper_PrintString("Child PID: ");
            syscall_wrapper_PrintInt(syscall_wrapper_GetPID());
            syscall_wrapper_PrintChar('\n');
            y++;
            syscall_wrapper_PrintString("Child2 y=");
            syscall_wrapper_PrintInt(y);
            syscall_wrapper_PrintChar('\n');
            syscall_wrapper_Exit(20);
        }
        else
        {
            syscall_wrapper_PrintString("Parent after fork waiting for child: ");
            syscall_wrapper_PrintInt(x);
            syscall_wrapper_PrintChar('\n');
            syscall_wrapper_PrintString("Parent2 join value: ");
            syscall_wrapper_PrintInt(syscall_wrapper_Join(x));
            syscall_wrapper_PrintChar('\n');
            syscall_wrapper_PrintString("Parent2 y=");
            syscall_wrapper_PrintInt(y);
            syscall_wrapper_PrintChar('\n');
            syscall_wrapper_Exit(10);
        }
    }
    else
    {
        syscall_wrapper_PrintString("Parent after fork waiting for child: ");
        syscall_wrapper_PrintInt(x);
        syscall_wrapper_PrintChar('\n');
        syscall_wrapper_PrintString("Parent2 join value: ");
        syscall_wrapper_PrintInt(syscall_wrapper_Join(x));
        syscall_wrapper_PrintChar('\n');
        syscall_wrapper_PrintString("Parent y=");
        syscall_wrapper_PrintInt(y);
        syscall_wrapper_PrintChar('\n');
        syscall_wrapper_Exit(1);
    }
    return 0;
}
