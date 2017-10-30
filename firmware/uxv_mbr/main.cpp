
#include <stdint.h>

#include "hal.h"

#include "usbcfg.h"
#include "flash_util.h"
#include "cmd_func.h"
#include "shell_func.h"
#include "uart_func.h"
#include "thread_func.h"



int main(void)
{

    halInit();

    chSysInit();

    // say hello
    palSetLine(LINE_LED1);
    palSetLine(LINE_LED2);

    // UART driver 3, PC10(TX) and PC11(RX) are routed to USART3.
    palSetPadMode(GPIOC, PIN_UART3_TX, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOC, PIN_UART3_RX, PAL_MODE_ALTERNATE(7));

#if ((HAL_USE_UART == TRUE) && (FW_SHELL_ON_SERIAL3 == TRUE))
#error  "UART3 config conflict"
#elif (HAL_USE_UART == TRUE)
    uartStart(&UARTD3, &uart3_cfg);
    uartStartSend(&UARTD3, 9, "\r\nmbr> \r\n");
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

    osalThreadSleepMilliseconds(500);

    if(palReadLine(LINE_BUT2) == LINE_BUT2_PRESSED) {
        cmd_func_goto_exec(BOOT_BASE);
    } else {
        cmd_func_goto_exec(APP1_BASE);
    }

}
