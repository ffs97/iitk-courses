#include "syscall.h"

#define OUTER_BOUND 4
#define SIZE 100

int
main() {
    int array[SIZE], i, k, sum;

    for (k = 0; k < OUTER_BOUND; k++)
    {
        for (i = 0; i < SIZE; i++) sum += array[i];
        syscall_wrapper_Yield();
    }
    return 0;
}
