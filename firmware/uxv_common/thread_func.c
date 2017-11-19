#include "ch.h"
#include "hal.h"

#include "usbcfg.h"
#include "thread_func.h"



/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg)
{

    (void)arg;
    chRegSetThreadName("blink");
    while(true) {
        systime_t time;

        time = serusbcfg.usbp->state == USB_ACTIVE ? 100 : 250;

        palClearLine(LINE_LED1);
        osalThreadSleepMilliseconds(time);
        palSetLine(LINE_LED1);
        osalThreadSleepMilliseconds(time);

        if(palReadLine(LINE_BUT1) == LINE_BUT1_PRESSED) {
            palSetLine(LINE_LED2);
        } else {
            palClearLine(LINE_LED2);
        }
    }
}


void CreateThread1(void)
{
    /*
     * Creates the blinker thread.
     */
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

}
