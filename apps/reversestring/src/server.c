/*
 * Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <camkes.h>
#include <buffer.h>
#include <string.h>
#include <stdio.h>

int run(void)
{

    char *buffer_str = (char *)buffer;

    while (buffer_str[REVERSE_STRING_END_IDX] == 0) {
        /* Poll the last byte of the buffer until the client is
         * ready. This acquire is necessary to prevent the compiler
         * from coalescing the reads and looping forever. */
        buffer_acquire();
    }

    printf("Got string: %s\n", buffer_str);

    int len = strnlen(buffer_str, REVERSE_STRING_MAX_LEN);
    for (int i = 0; i < len / 2; ++i) {
        int swap_idx = len - i - 1;
        char tmp = buffer_str[i];
        buffer_str[i] = buffer_str[swap_idx];
        buffer_str[swap_idx] = tmp;
    }

    /* The following write to the buffer signals to the client
     * that the server has finished reversing the string. This
     * release is necessary to prevent the following write
     * being re-ordered with the preceeding writes, which may
     * otherwise happen as they are non-overlapping. */
    buffer_release();

    buffer_str[REVERSE_STRING_END_IDX] = 0;

    return 0;
}
