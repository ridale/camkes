/*
 * Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/* CAmkES provides a generated header that prototypes all the relevant
 * generated symbols.
 */
#include <camkes.h>

#include <ringbuffer/ringbuffer.h>
#include <stdlib.h>

/* If we receive this character, interpret it as a request to switch outputs.
 */
#define SWITCH_CHAR '\t'

/* The current output we're sending data to. */
static enum {
    LOW,
    HIGH,
} selected = LOW;

/* This function is invoked by the main CAmkES thread in this component. */
int run(void)
{
    ringbuffer_t *low = rb_new((void *)low_output, sizeof(*low_output));
    if (low == NULL) {
        abort();
    }

    ringbuffer_t *high = rb_new((void *)high_output, sizeof(*high_output));
    if (high == NULL) {
        abort();
    }

    ringbuffer_t *input = rb_new((void *)char_in, sizeof(*char_in));
    if (input == NULL) {
        abort();
    }

    while (true) {
        char c = (char)rb_receive_byte(input);
//        printf("Switch received: %x\n", c);
        if (c == SWITCH_CHAR) {
//            printf("Switching!\n");
            /* Swap which output we see as active. */
            if (selected == LOW) {
                selected = HIGH;
            } else {
                selected = LOW;
            }
        } else {
            /* Send the character to the active output. */
            if (selected == LOW) {
                rb_transmit_byte(low, (unsigned char)c);
            } else {
                rb_transmit_byte(high, (unsigned char)c);
            }
        }
    }
    return 0;
}
