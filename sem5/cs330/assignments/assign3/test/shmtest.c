#include "syscall.h"

#define NUM_ITER 200

int main()
{
    int *array = (int *)syscall_wrapper_ShmAllocate(2 * sizeof(int));
    int x, i, y;

    array[0] = 0;
    array[1] = 100;

    x = syscall_wrapper_Fork();
    if (x == 0)
    {
        array[0] = 20;
        y = syscall_wrapper_Fork();
        for (i = 0; i < NUM_ITER; i++)
        {
            array[0]++;
        }
        syscall_wrapper_PrintString("Child Array value = ");
        syscall_wrapper_PrintInt(array[0]);
        if (y != 0)
        {
            syscall_wrapper_Join(y);
        }
    }
    else
    {
        array[1] = 200;
        for (i = 0; i < NUM_ITER; i++)
        {
            array[1]++;
        }
        x = syscall_wrapper_Join(x);
        syscall_wrapper_PrintString("Array[0]=");
        syscall_wrapper_PrintInt(array[0]);
        syscall_wrapper_PrintChar('\n');
        syscall_wrapper_PrintString("Array[1]=");
        syscall_wrapper_PrintInt(array[1]);
        syscall_wrapper_PrintChar('\n');
    }
    return 0;
}
