#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#if (HAL_USE_UART == TRUE)
uint32_t uart3_rxcount = 0;

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void uart3_txend1(UARTDriver* uartp)
{
    (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void uart3_txend2(UARTDriver* uartp)
{
    (void)uartp;
    // palSetPad(GPIOD, GPIOD_LED5);
    // chSysLockFromISR();
    // chVTResetI(&vt5);
    // chVTSetI(&vt5, MS2ST(200), led5off, NULL);
    // chSysUnlockFromISR();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void uart3_rxerr(UARTDriver* uartp, uartflags_t e)
{
    (void)uartp;
    (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void uart3_rxchar(UARTDriver* uartp, uint16_t c)
{
    (void)uartp;
    (void)c;

    uart3_rxcount++;

    /* Flashing the LED each time a character is received.*/
    palSetLine(LINE_LED2);
    // palSetPad(GPIOD, GPIOD_LED4);
    // chSysLockFromISR();
    // chVTResetI(&vt4);
    // chVTSetI(&vt4, MS2ST(200), led4off, NULL);
    // chSysUnlockFromISR();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void uart3_rxend(UARTDriver* uartp)
{
    (void)uartp;

    /* Flashing the LED each time a character is received.*/
    // palSetPad(GPIOD, GPIOD_LED3);
    // chSysLockFromISR();
    // chVTResetI(&vt3);
    // chVTSetI(&vt3, MS2ST(200), led3off, NULL);
    // chSysUnlockFromISR();
}

/*
 * UART driver configuration structure.  115200, 230400, 460800, 921600
 */
UARTConfig uart3_cfg = {
    uart3_txend1,
    uart3_txend2,
    uart3_rxend,
    uart3_rxchar,
    uart3_rxerr,
    115200,
    0,
    0,          // USART_CR2_LINEN,
    0
};

#endif
