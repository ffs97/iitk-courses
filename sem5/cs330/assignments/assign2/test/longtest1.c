#include "syscall.h"

#define OUTER_BOUND 1000

int
main() {
    int k;
    for (k = 0; k < OUTER_BOUND; k++) {
        if (k % 100 == 0) {
            syscall_wrapper_PrintChar('a');
            syscall_wrapper_PrintChar('b');
        }
    }
    return 0;
}
