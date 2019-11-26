/*
 * Copyright 2019, Penten
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 */
#include <camkes.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "packbuf.h"
#include "stats.h"

/**
 * Initialise the packet buffer
 */
int32_t pbuf_init(packbuf_t *pbuf, packet_t* const buffer) {
    if (pbuf == NULL || buffer == NULL) {
        return -1; // cant hold a packet
    }
    memset(pbuf, 0, sizeof(packbuf_t));
    memset(buffer, 0, PACKCAP*sizeof(packet_t));
    pbuf->buffer = buffer;
    return 0;
}

stats_t pack_stats = {0};
/**
 * Add a buffer to the buffer
 */
int32_t pbuf_raw_write(packbuf_t *pbuf, const uint8_t* const buf, const size_t len) {
    if (buf == NULL || pbuf == NULL || len == 0 || len > PACKLEN) {
        return -EINVAL;
    }

    packet_t* const empty = (packet_t* const)&pbuf->buffer[pbuf->current];
    if (empty->length != 0) {
        pbuf->dropped++;
        return -ENOSPC;
    }
    memcpy(empty->data, buf, len);
    empty->length = len; // set this last
    pbuf->current++;
    if (pbuf->current >= PACKCAP) {
        pbuf->current = 0;
    }
    pbuf->rx++;
    add_value(&pack_stats, (uint32_t)len);
    return 0;
}

/**
 * Add a packet to the buffer
 */
int32_t pbuf_write(packbuf_t *pbuf, const packet_t* const packet) {
    if (packet == NULL || pbuf == NULL || packet->length == 0 
                        || packet->length > PACKLEN) {
        return -EINVAL;
    }

    packet_t* const empty = (packet_t* const)&pbuf->buffer[pbuf->current];
    if (empty->length != 0) {
        pbuf->dropped++;
        return -ENOSPC;
    }
    memcpy(empty->data, packet->data, packet->length);
    empty->length = packet->length; // set this last
    pbuf->current++;
    if (pbuf->current >= PACKCAP) {
        pbuf->current = 0;
    }
    pbuf->rx++;
    return 0;
}
/**
 * Remove a packet from the buffer
 */
int32_t pbuf_read(packbuf_t *pbuf, packet_t* const packet) {
    if (packet == NULL || pbuf == NULL || packet->length != 0) {
        return -EINVAL;
    }

    packet_t* data = (packet_t*)&pbuf->buffer[pbuf->current];
    if (data->length == 0) {
        return -ENODATA;
    }
    *packet = *data; // struct assignment
    pbuf->current++;
    if (pbuf->current >= PACKCAP) {
        pbuf->current = 0;
    }
    memset(data, 0, sizeof(packet_t));
    pbuf->tx++;
    return 0;
}

/**
 * Check if buffer is full
 * @returns 1 if full, 0 if not full, -EINVAL on error
 */
int32_t pbuf_full(packbuf_t *pbuf) {
    if (pbuf == NULL) {
        return -EINVAL;
    }
    // check next index
    size_t index = pbuf->current + 1;
    if (index >= PACKCAP) {
        index = 0;
    }
    if (pbuf->buffer[index].length != 0) {
        return 1;
    }
    return 0;
}
/**
 * Check if buffer is empty
 * @returns 1 if empty, 0 if not empty, -EINVAL on error
 */
int32_t pbuf_empty(packbuf_t *pbuf) {
    if (pbuf == NULL) {
        return -EINVAL;
    }
    // check previous index
    size_t index = pbuf->current - 1;
    if (index >= PACKCAP) {
        // wrapped negative size_t
        index = PACKCAP -1;
    }
    if (pbuf->buffer[index].length == 0) {
        return 1;
    }
    return 0;
}