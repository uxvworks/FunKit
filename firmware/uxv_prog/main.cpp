

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "usbcfg.h"
#include "flash_util.h"
#include "cmd_func.h"
#include "shell_func.h"
#include "uart_func.h"
#include "thread_func.h"
#include "cfg_storage.h"

#include "ws2812/ws2812.h"


#if (FW_SHELL_APPCMD_ENABLED == TRUE)
void sh_cmd_appcmd(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    const char *const usage =
        "Usage: cmd (led) (NNRRGGBB) in 32 bit Hex format\r\n";
    static bool init_done = false;
    uint32_t NRGB = 0;
    // int base = 10;
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

            // int s = 0;
            uint8_t blu = NRGB & 0x000000FF;
            uint8_t grn = (NRGB >> 8)  & 0x000000FF;
            uint8_t red = (NRGB >> 16) & 0x000000FF;
            uint32_t  n = (NRGB >> 24) & 0x000000FF;

            chprintf(chp, "NRGB:%08X led:%u r:%d g:%d b:%d\r\n", NRGB, n, red, grn, blu);
            // uint32_t cnt;

            //for (; n < WS2812_LED_N; n++) {
            ws2812_write_led(n, red, grn, blu);
            // }

#if 0
            for (cnt = 0; cnt < count; cnt++)  {
                for (n = 0; n < WS2812_LED_N; n++) {
                    int s0 = s + 10*n;
                    ws2812_write_led(n, s0%255, (s0+85)%255, (s0+170)%255);
                }
                s += 10;
                osalThreadSleepMilliseconds(50);
            }
#endif

            retval = 0;  //ping_cli(chp, count);
            chprintf(chp, "cmd led returned  %d\r\n", retval);
        }
    } else {
        chprintf(chp, usage);
    }
}
#endif



int main(void)
{
    halInit();
    chSysInit();

    cfg_storage_init();

    // UART driver 3, PC10(TX) and PC11(RX) are routed to USART3.
    palSetPadMode(GPIOC, PIN_UART3_TX, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOC, PIN_UART3_RX, PAL_MODE_ALTERNATE(7));

#if ((HAL_USE_UART == TRUE) && (FW_SHELL_ON_SERIAL3 == TRUE))
#error  "UART3 config conflict"
#elif (HAL_USE_UART == TRUE)
    uartStart(&UARTD3, &uart3_cfg);
    uartStartSend(&UARTD3, 12, "\r\nuxv_prog\r\n");
#elif (HAL_USE_SERIAL == TRUE)
    static SerialConfig ser3_cfg = {
        115200,
        0,
        0,
        0,
    };
    sdStart(&SD3, &ser3_cfg);
#endif


#if (HAL_USE_SERIAL_USB == true)
    /*
     * Initializes a serial-over-USB CDC driver.
     */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus(serusbcfg.usbp);
    osalThreadSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
#endif

    /*
     * Shell manager initialization.
     */
    shellInit();

    CreateThread1();

#if (FW_SHELL_ON_SERIAL3 == TRUE)
    //thread_t* shelltp_sd3 =
    chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shellsd3", NORMALPRIO + 1, shellThread, (void*)&shell_cfg_sd3);
    //chThdWait(shelltp_sd3); /* Waiting termination.             */
#endif
    while(true) {
#if (FW_SHELL_ON_USB == TRUE)
        if(SDU1.config->usbp->state == USB_ACTIVE)  {
            thread_t* shelltp_sdu =
                chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shellsdu", NORMALPRIO + 1, shellThread, (void*)&shell_cfg_sdu);
            chThdWait(shelltp_sdu); /* Waiting termination.             */
        }
#endif
        osalThreadSleepMilliseconds(1000);
    }
}

/*
      chEvtWaitAny(EVENT_MASK(0));
      if (chThdTerminatedX(shelltp1)) {
        chThdRelease(shelltp1);
        shelltp1 = NULL;
      }
*/
