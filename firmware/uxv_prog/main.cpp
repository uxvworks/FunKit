

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"

#include "shellconf.h"
#include "chprintf.h"
#include "shell.h"

#include "portab.h"
#include "usbcfg.h"

//#include "flash_util.h"
#include "cmd_func.h"
#include "shell_func.h"
//#include "uart_func.h"
#include "thread_func.h"
#include "cfg_storage.h"

#include "ws2812/ws2812.h"
#include "sdcard/sdcard_mgr.h"

//static void *testptr = NULL;
/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/


#if (FW_SHELL_APPCMD_ENABLED == TRUE)
void sh_cmd_appcmd(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    const char *const usage =
        "Usage: cmd (led) (NNRRGGBB) in 32 bit Hex format\r\n";
    static bool init_done = false;
    uint32_t NRGB = 0;
    char *endptr;
    int retval = 0;

    if(argc < 1) {
        chprintf(chp, usage);
        return;
    } else if (streq(argv[0], "led") && (argc == 2)) {
        NRGB = (uint32_t)strtoul(argv[1], &endptr, 16);
        if (argv[1] == endptr) {
            chprintf(chp, "No NRGB was entered?\r\n");
            chprintf(chp, usage);
        } else {
            if (!init_done) {
                ws2812_init();
                init_done = true;
            }

            uint8_t blu = NRGB & 0x000000FF;
            uint8_t grn = (NRGB >> 8)  & 0x000000FF;
            uint8_t red = (NRGB >> 16) & 0x000000FF;
            uint32_t  n = (NRGB >> 24) & 0x000000FF;

            chprintf(chp, "NRGB:%08X led:%u r:%d g:%d b:%d\r\n", NRGB, n, red, grn, blu);

            if (n == 0xFF) {
                uint32_t index;
                for (index = 0; index < WS2812_LED_N; index++) {
                    ws2812_write_led(index, red, grn, blu);
                }
                //testptr = chHeapAlloc(NULL, 1000);
            } else if (n == 0xFD) {
                uint32_t index;
                uint32_t eye =6;
                for (index = 0; index < 100; index++) {
                    ws2812_write_led(eye, red, grn, blu);
                    osalThreadSleepMilliseconds(2000);
                    ws2812_write_led(eye, 0, 0, 0);
                    osalThreadSleepMilliseconds(100);
                    if (eye == 0x06)  eye = 0x07;
                    else if (eye ==0x07) eye = 0x08;
                    else eye = 0x06;
                }
            } else if (n == 0xFC) {
                uint32_t index;
                while (true) {
                    for (index = 0; index < WS2812_LED_N; index++) {
                        ws2812_write_led(index, red, grn, blu);
                        osalThreadSleepMilliseconds(5);
                        ws2812_write_led(index, 0, 0, 0);
                    }
                    for (index = WS2812_LED_N; index > 0; index--) {
                        ws2812_write_led(index-1, red, grn, blu);
                        osalThreadSleepMilliseconds(5);
                        ws2812_write_led(index-1, 0, 0, 0);
                    }
                    if (palReadLine(LINE_BUT1) == LINE_BUT1_PRESSED) break;
                }
            } else if (n == 0xFB) {
                uint32_t index;
                while (true) {
                    for (index = 10; index > 0; index--) {
                        float redf = float(index * red) * 0.1;
                        float grnf = float(index * grn) * 0.1;
                        float bluf = float(index * blu) * 0.1;
                        uint32_t index2;
                        for (index2 = 0; index2 < WS2812_LED_N; index2++) {
                            ws2812_write_led(index2, (uint8_t)redf, (uint8_t)grnf, (uint8_t)bluf);
                        }
                        osalThreadSleepMilliseconds(50);
                    }
                    if (palReadLine(LINE_BUT1) == LINE_BUT1_PRESSED) break;
                    for (index = 1; index <= 10; index++) {
                        float redf = float(index * red) * 0.1;
                        float grnf = float(index * grn) * 0.1;
                        float bluf = float(index * blu) * 0.1;
                        uint32_t index2;
                        for (index2 = 0; index2 < WS2812_LED_N; index2++) {
                            ws2812_write_led(index2, (uint8_t)redf, (uint8_t)grnf, (uint8_t)bluf);
                        }
                        osalThreadSleepMilliseconds(50);
                    }
                }
            } else {
                ws2812_write_led(n, red, grn, blu);
                //if (testptr) chHeapFree(testptr);
            }

            retval = 0;
            chprintf(chp, "cmd led returned  %d\r\n", retval);
        }
    } else {
        chprintf(chp, usage);
    }
}
#endif



/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

static thread_t *shelltp = NULL;

/*
 * Shell exit event.
 */
static void ShellHandler(eventid_t id)
{

    (void)id;
    if (chThdTerminatedX(shelltp)) {
        chThdRelease(shelltp);
        shelltp = NULL;
    }
}


/*
 * Application entry point.
 */
int main(void)
{
    static const evhandler_t evhndl[] = {
        sdcard_InsertHandler,
        sdcard_RemoveHandler,
        ShellHandler
    };

    halInit();
    chSysInit();
    
   // cfg_storage_init();
    
    portab_setup();   //Target-dependent setup code.

#if ((HAL_USE_UART == TRUE) && (FW_SHELL_ON_SERIAL == TRUE))
#error  "UART config conflict"
#elif (HAL_USE_UART == TRUE)
    uartStart(&UARTD3, &uart3_cfg);
    uartStartSend(&UARTD3, 12, "\r\nuxv_prog\r\n");
#elif (HAL_USE_SERIAL == TRUE)
    static SerialConfig ser_cfg = {
        115200,
        0,
        0,
        0,
    };
    sdStart(&PORTAB_SERIAL, &ser_cfg);
#endif


#if (HAL_USE_SERIAL_USB == true)
    /*
     * Initializes a serial-over-USB CDC driver.
    */
    sduObjectInit(&PORTAB_SDU1);
    sduStart(&PORTAB_SDU1, &serusbcfg);
    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
    */
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
#endif

    shellInit();

#if (FW_INCLUDE_SDCARD_MGR == TRUE)
    event_listener_t el0, el1;
    sdcard_mgr_start();
    chEvtRegister(&sdcard_inserted_event, &el0, 0);
    chEvtRegister(&sdcard_removed_event, &el1, 1);
#endif

#if (FW_SHELL_ON_SERIAL == TRUE)
    chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shellserial", NORMALPRIO + 1, shellThread, (void*)&shell_cfg_serial);
#endif

    CreateThread1();

    //httpd_init();  // Starts the HTTP server.

    event_listener_t el2;
    chEvtRegister(&shell_terminated, &el2, 2);

    while (true) {
        if (!shelltp && (PORTAB_SDU1.config->usbp->state == USB_ACTIVE)) {
            shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                          "shellusb", NORMALPRIO + 1,
                                          shellThread, (void *)&shell_cfg_sdu);
        }
        chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
    }
}
