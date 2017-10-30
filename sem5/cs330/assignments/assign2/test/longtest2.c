#include "syscall.h"

#define OUTER_BOUND 10

int
main() {
    int k;
    for (k = 0; k < OUTER_BOUND; k++) {
        syscall_wrapper_PrintInt(2);
    }
    return 0;
}
