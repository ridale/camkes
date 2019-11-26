/*
 * Copyright 2019, Penten
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 */
#pragma once

#include <stddef.h>
#include <inttypes.h>

/**
 * A fifo packet
 */
typedef struct stats {
    double M;
    uint32_t N;
} stats_t;


static void add_value(stats_t* s, uint32_t value) {
    if (s->N == 0){
        s->N++;
        s->M = value;
    }
    else {
        s->N++;
        s->M = s->M + (value - s->M) / s->N;
    }
}

static int32_t calc_mean(const stats_t* const s) {
    return (int32_t)(s->M);
}