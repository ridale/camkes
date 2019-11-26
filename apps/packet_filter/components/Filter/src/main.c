/*
 * Copyright 2019, Penten
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * Filter Cell, handles the packet filtering
 *
 */

#include <camkes.h>
#include <stdio.h>
#include <string.h>
#include <camkes/dataport.h>

#include "packbuf.h"
#include "eth.h"

packbuf_t in_bufferlo = {0};
packbuf_t out_bufferlo = {0};

packbuf_t in_bufferhi = {0};
packbuf_t out_bufferhi = {0};



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
uint32_t inboundlo_xfer() {
    // unblock mutex
    int ret = semlo_post();
    return 0;
}
/**
 * Read data passed in from the other eth cell
 */
uint32_t inboundhi_xfer() {
    // unblock mutex
    int ret = semhi_post();
    return 0;
}
/**
 * Run prior to the start of main
 */
void post_init() {
    pbuf_init(&out_bufferlo, outdatalo);
    pbuf_init(&in_bufferlo, indatalo);
    pbuf_init(&out_bufferhi, outdatahi);
    pbuf_init(&in_bufferhi, indatahi);
}
/**
 * Cell main run function
 */
int run(void) {
    ZF_LOGD("run");
    int32_t ret = 0;

    packet_t inpack = {0};
    packet_t outpack = {0};
    while (true) {

        ret = semhi_wait();
        indatahi_acquire();
        do {
            ret = pbuf_read(&in_bufferhi, &inpack);
            if (0 == ret) {
                pbuf_write(&out_bufferlo,  &inpack);
                indatahi_release();
                outdatalo_release();
            }
            inpack.length = 0;
            outpack.length = 0;
        }
        while (ret == 0);
        outboundlo_xfer();
    }
}
