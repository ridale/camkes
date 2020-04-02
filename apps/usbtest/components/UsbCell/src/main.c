
#include <camkes.h>
#include <autoconf.h>
#include <stdio.h>

#define USB_TICK_MS 1000

void pre_init(void)
{
    //usb_init();
    /* Start the timer for the TCP stack */
    timer_periodic(0, NS_IN_MS * USB_TICK_MS);
}

/* Callback that gets called when the timer fires. */
void timer_complete_callback(void)
{
    int err = mut_lock();
    // check stuff
    //printf("usbing\n");
    err = mut_unlock();
}


int run(void) {
    return 0;
}
