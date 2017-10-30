
#include <stdio.h>
#include <string.h>

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
    uartStartSend(&UARTD3, 12, "\r\nuxv_boot\r\n");
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
