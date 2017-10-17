#include "syscall.h"

int
main()
{
    syscall_wrapper_PrintString("Before calling Exec.\n");
    syscall_wrapper_Exec("../test/vectorsum");
    syscall_wrapper_PrintString("Returned from Exec.\n"); // Should never return
    return 0;
}
