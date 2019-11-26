#include <autoconf.h>

#include <string.h>
#include <stdio.h>
#include <camkes.h>
#include <sel4/sel4.h>
#include <sel4utils/sel4_zf_logif.h>

#include <platsupport/io.h>
#include <ethdrivers/raw.h>
#include <ethdrivers/helpers.h>

#include "packbuf.h"

extern packbuf_t in_buffer;

static void low_level_init(struct eth_driver *driver, uint8_t *mac, int *mtu)
{
    // 1500 is the standard ethernet MTU at the network layer.
    *mtu = 1500;
    ethdriver_mac(&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
}

extern void *ethdriver_buf;

static void raw_poll(struct eth_driver *driver)
{
    int len = 0;
    int status = -1;
    packet_t pack = {0};
    while (pbuf_full(&in_buffer) == 0) {            // while we have space
        status = ethdriver_rx(&len);                // read from DMA
        ZF_LOGD("len=%i status=%i",len,status);
        if (status == 1) {
            if (len < PACKLEN && len > 0) {
                pack.length = len;
                if (pbuf_raw_write(&in_buffer, (uint8_t*)ethdriver_buf, len) != 0) {
                    status = -1; // buffer full
                    continue;
                }
            }
        }
        else {
            // status must be 0, we have already
            // read this packet or -1 read failed
            break;                                  // until no data left
        }
    }
}

static int raw_tx(struct eth_driver *driver, unsigned int num, uintptr_t *phys, unsigned int *len, void *cookie)
{
    unsigned int total_len = 0;
    int i;
    void *p = (void *)ethdriver_buf;
    for (i = 0; i < num; i++) {
        memcpy(p + total_len, (void *)phys[i], len[i]);
        total_len += len[i];
    }
    ethdriver_tx(total_len);
    return ETHIF_TX_COMPLETE;
}

void handle_irq(struct eth_driver *driver, int irq)
{
    ZF_LOGV("irq fired");
    raw_poll(driver);
}

static struct raw_iface_funcs iface_fns = {
    .raw_handleIRQ = handle_irq,
    .print_state = NULL,
    .low_level_init = low_level_init,
    .raw_tx = raw_tx,
    .raw_poll = raw_poll
};

static int ethdriver_init(struct eth_driver *eth_driver, ps_io_ops_t io_ops, void *config)
{
    eth_driver->eth_data = NULL;
    eth_driver->dma_alignment = 1;
    eth_driver->i_fn = iface_fns;
    return 0;
}

static void *malloc_dma_alloc(void *cookie, size_t size, int align, int cached, ps_mem_flags_t flags)
{
    assert(cached);
    int error;
    void *ret = malloc(size);
    if (ret == NULL) {
        ZF_LOGE("ERR: Failed to allocate %d\n", size);
        return NULL;
    }
    return ret;
}

static void malloc_dma_free(void *cookie, void *addr, size_t size)
{
    free(addr);
}

static uintptr_t malloc_dma_pin(void *cookie, void *addr, size_t size)
{
    return (uintptr_t)addr;
}

static void malloc_dma_unpin(void *cookie, void *addr, size_t size)
{
}

static void malloc_dma_cache_op(void *cookie, void *addr, size_t size, dma_cache_op_t op)
{
}
static void tx_complete(void *iface, void *cookie) {
    //free_buf_pool(iface, (long) cookie);
    ZF_LOGD("TX complete!");//, ((pico_device_eth*)iface)->rx_count);
}
static void rx_complete(void *iface, unsigned int num_bufs, void **cookies, unsigned int *lens) {
    // queue the buffer
    ZF_LOGD("RX complete!");//, ((pico_device_eth*)iface)->rx_count);
}
static uintptr_t allocate_rx_buf(void *iface, size_t buf_size, void **cookie) {
    ZF_LOGD("allocate_rx_buf");

    return 0; // no buffers available
}
static ps_io_ops_t io_ops;
static struct raw_iface_callbacks prealloc_callbacks = {
    .tx_complete = tx_complete,
    .rx_complete = rx_complete,
    .allocate_rx_buf = allocate_rx_buf
};

void eth_init(struct eth_driver *driver)
{
    ZF_LOGD("eth_init start!\n");
    memset(&io_ops, 0, sizeof(io_ops));
    io_ops.dma_manager = (ps_dma_man_t) {
        .cookie = NULL,
        .dma_alloc_fn = malloc_dma_alloc,
        .dma_free_fn = malloc_dma_free,
        .dma_pin_fn = malloc_dma_pin,
        .dma_unpin_fn = malloc_dma_unpin,
        .dma_cache_op_fn = malloc_dma_cache_op
    };
    driver->i_cb = prealloc_callbacks;
    driver->cb_cookie = NULL;
    ethdriver_init(driver, io_ops, NULL);

    int mtu;
    uint8_t mac[6] = {0};
    driver->i_fn.low_level_init(driver, mac, &mtu);
    ZF_LOGD("eth_init complete!\n");
}