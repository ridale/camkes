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
#include <stdbool.h>
#include <arpa/inet.h>

#include <camkes.h>

typedef struct __attribute__((__packed__)) eth_hdr {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
} eth_hdr_t;

typedef struct __attribute__((__packed__)) eth_vlan_hdr {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t id_8021q;
    uint16_t tci_8021q;
    uint16_t type;
} eth_vlan_hdr_t;

/**
 * Get a vlan ID from tag control info field
 */
static inline uint32_t get_vid(uint16_t tci) {
    return tci&0xFFF;
}
/**
 * Get 802.1Q priority from tag control info field
 * NOTE LSB == drop eligible indicator
 */
static inline uint8_t get_prio(uint16_t tci) {
    return tci&0xF000 >>12;
}
