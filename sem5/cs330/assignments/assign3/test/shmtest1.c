#include "syscall.h"
#include "synchop.h"

#define NUM_ITER 200

void DekkerEntry(int *flag, int id, int *turn) {
    flag[id] = 1;
    while (flag[1 - id])
    {
        if ((*turn) == (1 - id))
        {
            flag[id] = 0;
            while ((*turn) == (1 - id));
            flag[id] = 1;
        }
    }
}

void DekkerExit(int *flag, int id, int *turn) {
    (*turn) = 1 - id;
    flag[id] = 0;
}

int
main() {
    int *array = (int *) syscall_wrapper_ShmAllocate(4 * sizeof(int)); // count, flag[0], flag[1], turn
    int x, i;

    for (i = 0; i < 4; i++) array[i] = 0;

    x = syscall_wrapper_Fork();
    if (x == 0)
    {
        for (i = 0; i < NUM_ITER; i++)
        {
            DekkerEntry(&array[1], 1, &array[3]);
            array[0]++;
            DekkerExit(&array[1], 1, &array[3]);
        }
    }
    else
    {
        for (i = 0; i < NUM_ITER; i++)
        {
            DekkerEntry(&array[1], 0, &array[3]);
            array[0]++;
            DekkerExit(&array[1], 0, &array[3]);
        }
        x = syscall_wrapper_Join(x);
        syscall_wrapper_PrintString("Array[0]=");
        syscall_wrapper_PrintInt(array[0]);
        syscall_wrapper_PrintChar('\n');
    }
    return 0;
}
