/*
 * Copyright 2019, Penten
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * Ethernet driver cell, gets/sends packets from HW DMA
 * 
 */

#include <camkes.h>
#include <camkes/io.h>
#include <stdio.h>
#include <string.h>
#include <camkes/dataport.h>
#include <usb/usb.h>

#include "packbuf.h"

packbuf_t in_buffer = {0};
packbuf_t out_buffer = {0};


static void hexdump(const uint8_t* const data, const size_t size) {
	size_t i, j;
    if (size == 0) printf("%06X  \n", size);
	for (i = 0; i < size; ++i) {
        if (i % 16 == 0) {
            printf("%06X  ", i);
        }
		printf("%02X ", ((unsigned char*)data)[i]);
		if ((i+1) % 16 == 0 || i+1 == size) {
			printf("\n");
		}
	}
}

/**
 * Read data passed in from the other eth cell
 */
uint32_t outboundlo_xfer() {
    // unblock mutex
    int ret = sem_post();
    return 0;
}
static uint32_t counter = 0;
/**
 * seL4 interrupt called by epit cell every 100ms(ish)
 */
static void tick100ms(void *arg) {
    indatalo_acquire();
    if (pbuf_empty(&in_buffer) == 0) {
        inboundlo_xfer();
    }
    indatalo_release();
    int error = tick100ms_reg_callback(tick100ms, NULL);
}

/**
 * Run prior to the start of main
 */
void post_init() {

    pbuf_init(&in_buffer, indatalo);
    pbuf_init(&out_buffer, outdatalo);
}
/**
 * Cell main run function
 */
int run(void) {
    ZF_LOGD("run");
    int error = tick100ms_reg_callback(tick100ms, NULL);
    int32_t ret = 0;
    packet_t pack = {0};
    while (true) {
        ret = sem_wait();
        pack.length = 0;
        outdatalo_acquire();
        do {
            ret = pbuf_read(&out_buffer, &pack);
            if (0 == ret) {
                // raw tx the eth, or just hexdump
                hexdump(pack.data, pack.length);
            }
            pack.length = 0;
            outdatalo_release();
        }
        while (ret == 0);
    }
}
