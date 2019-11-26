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
#include <errno.h>

#define PACKLEN 1504U   // padded by 4 bytes to align
#define PACKCAP 85U     // capacity of the circbuffer

/**
 * A fifo packet
 */
typedef struct packet_s {
    size_t length;
    uint8_t data[PACKLEN];
} packet_t;

/**
 * The fifo context
 */
typedef struct packbuf_s {
    size_t dropped;
    size_t tx;
    size_t rx;
    size_t current;
    packet_t *buffer;
} packbuf_t;


int32_t pbuf_init(packbuf_t *pbuf, packet_t* const buffer);
int32_t pbuf_write(packbuf_t *pbuf, const packet_t* const packet);
int32_t pbuf_raw_write(packbuf_t *pbuf, const uint8_t* const buf, const size_t len);
int32_t pbuf_read(packbuf_t *pbuf, packet_t* const packet);
int32_t pbuf_full(packbuf_t *pbuf);
int32_t pbuf_empty(packbuf_t *pbuf);
