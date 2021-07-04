/*
 * Copyright 2017, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <camkes.h>

static int global;

static void callback(void *arg)
{
    int *ret = arg;
    *ret = global;
    int error = binsem_post();
    assert(!error);
}

int echo_echo(int i)
{
    int ret;
    global = i;
    int error = ready_reg_callback(callback, &ret);
    assert(!error);
    error = binsem_wait();
    assert(!error);
    return ret;
}
