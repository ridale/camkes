/*
 * Copyright 2018, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <autoconf.h>
#include <camkes.h>
#include <stdio.h>

#define WAIT_100MS (100 * NS_IN_MS) /* amount of time to set timeouts for */

seL4_CPtr timeout_notification(void);

int run(void)
{
    seL4_Word badge;
    seL4_CPtr notification = timeout_notification();

    /* test we can read the time */
    uint64_t t1 = timeout_time();

    uint64_t index = 0U;

    /* test a periodic timeout */
    timeout_periodic(0, WAIT_100MS);
    while (true) {
        seL4_Wait(notification, &badge);
		index++;
	    tick100ms_emit();
		if (index == 10) {
			index = 0;
		    tick1s_emit();
		}
    }

    return 0;
}
