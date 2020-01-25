
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "usbcfg.h"
#include "cmd_func.h"
#include "flash_defines.h"
#include "flash_util.h"
#include "uart_func.h"
#include "cfg_storage.h"
#include "stm_eeprom.h"
#include "shell_func.h"




/*===========================================================================*/
/* Shell Command line related.                                                     */
/*===========================================================================*/

#if 0
#define SEND_MSG_BUF_SIZE 64
static uint8_t send_message[SEND_MSG_BUF_SIZE] = "\r\n";
#endif

#if (FW_SHELL_EEPROM_ENABLED == TRUE) 
void sh_cmd_eeprom(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;

    const char *const usage =
        "Usage: eeprom (r|w) (address) (value)\r\n";
    uint16_t value = 0;
    uint16_t address = 0;
    int base = 16;
    char *endptr;

    if  (argc < 1) {
        chprintf(chp, usage);
        return;
    }   else if (streq(argv[0], "r") && (argc == 2)) {
        address = (uint16_t)strtoul(argv[1], &endptr, base);
        if (argv[1] == endptr) {
            chprintf(chp, "No address was entered?\r\n");
            chprintf(chp, usage);
        } else {
            if (EE_ReadVariable(address, &value) == EE_OK) {
                chprintf(chp, "EEPROM address %04X is %04X\r\n", address, value);
            } else {
                chprintf(chp, "EEPROM read error at %04X\r\n", address);
            }
        }
    } else if (streq(argv[0], "w") && (argc == 3)) {
        address = (uint32_t)strtoul(argv[1], &endptr, base);
        if (argv[1] == endptr) {
            chprintf(chp, "No address was entered?\r\n");
            chprintf(chp, usage);
        } else {
            value = (uint32_t)strtoul(argv[2], &endptr, base);
            if (argv[2] == endptr) {
                chprintf(chp, "No value was entered?\r\n");
                chprintf(chp, usage);
            } else {
                if (EE_WriteVariable(address, value) == EE_OK) {
                    chprintf(chp, "EEPROM write OK at %04X to %04X\r\n", address, value);
                } else {
                    chprintf(chp, "EEPROM write error at %04X\r\n", address);
                }
            }
        }
    } else {
        chprintf(chp, usage);
        return;
    }

    chprintf(chp, "Cfg_Store size:%d  NUM_VARS:%d\r\n", sizeof(app_cfg_t), EE_NUM_VARS);
}
#endif


#if (FW_SHELL_FLASH_ENABLED == TRUE) 
void sh_cmd_flash(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;

    const char *const usage =
        "Usage: flash (e|r|w) (value)\r\n";
    uint32_t value = 0xFFFFFFFF;
    int base = 16;
    char *endptr;

    if  (argc < 1) {
        chprintf(chp, usage);
        return;
    } else if (streq(argv[0], "e")) {
        chprintf(chp, "Erasing flash %08X to %08X\r\n", FLASH_USER_START_ADDR, FLASH_USER_END_ADDR);
        flash_erase_user_data();
    }   else if (streq(argv[0], "r") && (argc == 2)) {
        value = (uint32_t)strtoul(argv[1], &endptr, base);
        if (argv[1] == endptr) {
            chprintf(chp, "No test value was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Verifying flash is %08X\r\n", value);
            //verify done on exit
        }
    } else if (streq(argv[0], "w") && (argc == 2)) {
        value = (uint32_t)strtoul(argv[1], &endptr, base);
        if (argv[1] == endptr) {
            chprintf(chp, "No test value was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Setting flash to %08X\r\n", value);
            flash_store_user_data(value);
        }
    } else {
        chprintf(chp, usage);
        return;
    }

    chprintf(chp, "flash verify for %08X, errors = %08X\r\n", value, flash_verify_user_data(value));
}
#endif


void sh_cmd_otp(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;

    const char *const usage =
        "Usage: otp (r|w) (serial) (device) (id16) (id8)\r\n";
    uint32_t number;
    int base = 16;
    char *endptr;

    if  (argc < 1) {
        chprintf(chp, usage);
        return;
    } else if (streq(argv[0], "r") ) {
        ;; //chprintf(chp, "Serial = %08X\r\n", cmd_func_otp_get_serial());
    } else if (streq(argv[0], "w") && (argc == 5)) {
        // number = (uint32_t)atoi(argv[1]);
        number = (uint32_t)strtoul(argv[1], &endptr, base);
        if (argv[1] == endptr) {
            chprintf(chp, "No serial number was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Setting serial to %08X\r\n", number);
            cmd_func_otp_set_serial(number);
        }
        //} else if (streq(argv[2], "d") && (argc == 5)) {
        number = (uint32_t)strtoul(argv[2], &endptr, base);
        if (argv[2] == endptr) {
            chprintf(chp, "No device number was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Setting device to %08X\r\n", number);
            cmd_func_otp_set_device(number);
        }
        //} else if (streq(argv[4], "i16") && (argc == 5)) {
        number = (uint32_t)strtoul(argv[3], &endptr, base);
        if (argv[3] == endptr) {
            chprintf(chp, "No id16 number was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Setting id16 to %08X\r\n", number);
            cmd_func_otp_set_id16(number);
        }
        //} else if (streq(argv[6], "i8") && (argc == 5)) {
        number = (uint32_t)strtoul(argv[4], &endptr, base);
        if (argv[4] == endptr) {
            chprintf(chp, "No id8 number was entered?\r\n");
            chprintf(chp, usage);
        } else {
            chprintf(chp, "Setting id8 to %08X\r\n", number);
            cmd_func_otp_set_id8(number);
        }

    } else {
        chprintf(chp, usage);
        return;
    }

    // chprintf(chp, "lock0  = %02X\r\n", (uint32_t)cmd_func_otp_get_lock(0));
    chprintf(chp, "serial = %08X\r\n", cmd_func_otp_get_serial());
    chprintf(chp, "device = %08X\r\n", cmd_func_otp_get_device());
    chprintf(chp, "id16   = %08X\r\n", cmd_func_otp_get_id16());
    chprintf(chp, "id8    = %08X\r\n", cmd_func_otp_get_id8());
    chprintf(chp, "lock0  = %02X\r\n", cmd_func_otp_get_lock(0));
    //chprintf(chp, "name   = %s\r\n",   cmd_func_otp_get_name());
}


void sh_cmd_uid(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: uid\r\n");
        return;
    }

    //uint32_t uid0;
    //uint32_t uid1;
    //uint32_t uid2;
    // cmd_func_uid(&uid0, &uid1, &uid2) ;

    uint32_t uid[3];
    cmd_func_uid(uid) ;

    // chprintf(chp, "UID = %08X:%08X:%08X\r\n", uid2, uid1, uid0);
    chprintf(chp, "UID = %08X:%08X:%08X\r\n", uid[2], uid[1], uid[0]);
}

#if (FW_SHELL_GO2APP1_ENABLED == TRUE) 
void sh_cmd_startapp1(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: app1\r\n");
        return;
    }
    chprintf(chp, "Will start app1 in 1 second\r\n");
    osalThreadSleepMilliseconds(1000);

    cmd_func_goto_exec(APP1_BASE_ADDR);
}
#endif

#if (FW_SHELL_GO2BOOT_ENABLED == TRUE) 
void sh_cmd_startboot(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: boot\r\n");
        return;
    }
    chprintf(chp, "Will start bootloader in 1 second\r\n");
    osalThreadSleepMilliseconds(1000);

    cmd_func_goto_exec(BOOT_BASE_ADDR);
}
#endif

void sh_cmd_reset(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: reset\r\n");
        return;
    }

    chprintf(chp, "Will reset in 1 second\r\n");
    osalThreadSleepMilliseconds(1000);

    cmd_func_reset();
}


#if (FW_SHELL_U3SEND_ENABLED == TRUE) 
static void sh_cmd_uart3_send(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: u3send\r\n");
        return;
    }
#if (HAL_USE_UART == TRUE)
    uartStartSend(&UARTD3, 7, "UART3\r\n");
#else
    chprintf(chp, "UARTD3 not available.\r\n");
#endif
}
#endif


#if (FW_SHELL_WRITE_ENABLED == TRUE) 
/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void sh_cmd_write(BaseSequentialStream* chp, int argc, char* argv[])
{
    static uint8_t buf[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
                           "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: write\r\n");
        return;
    }

    uint32_t loop_cnt = 0;

    while(chnGetTimeout((BaseChannel*)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
#if 1
        /* Writing in channel mode.*/
        chnWrite(chp /*&SDU1*/, buf, sizeof buf - 1);
#else
        /* Writing in buffer mode.*/
        (void)obqGetEmptyBufferTimeout(&SDU1.obqueue, TIME_INFINITE);
        memcpy(SDU1.obqueue.ptr, buf, SERIAL_USB_BUFFERS_SIZE);
        obqPostFullBuffer(&SDU1.obqueue, SERIAL_USB_BUFFERS_SIZE);
#endif
        loop_cnt++;
        if (palReadLine(LINE_BUT1) == LINE_BUT1_PRESSED) break;
    }
    chprintf(chp, "\r\n\nstopped after %u X %u bytes.\r\n", loop_cnt, sizeof buf-1 );
}
#endif

#if (FW_SHELL_READ_ENABLED == TRUE) 
static void sh_cmd_read(BaseSequentialStream* chp, int argc, char* argv[])
{
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: read\r\n");
        return;
    }

    uint32_t loop_cnt = 0;
    uint32_t byte_total = 0;

    uint8_t* buf = chHeapAlloc(NULL, 256);

    chprintf(chp, "Reading from port until button pressed...\r\n");

    while (palReadLine(LINE_BUT1) != LINE_BUT1_PRESSED) {

        size_t retval = chnReadTimeout((BaseChannel*)chp, buf, sizeof buf, OSAL_MS2ST(1));

        if(retval > 0) {
            byte_total += retval;
            //systime_t now_time = osalOsGetSystemTimeX();
            //chprintf(chp, "%u %u rx: %u\r\n", now_time, loop_cnt, retval);
        }

        loop_cnt++;
    }

    chHeapFree(buf);
    chprintf(chp, "\r\n\nstopped after %u bytes.\r\n", byte_total );
}
#endif

static const ShellCommand commands[] = {
#if (FW_SHELL_READ_ENABLED == TRUE) 
    { "read", sh_cmd_read },
#endif
#if (FW_SHELL_WRITE_ENABLED == TRUE)     
    { "write", sh_cmd_write },
#endif
    { "uid", sh_cmd_uid },
    { "otp", sh_cmd_otp },
#if (FW_SHELL_FLASH_ENABLED == TRUE)    
    { "flash", sh_cmd_flash },
#endif
#if (FW_SHELL_EEPROM_ENABLED == TRUE)     
    { "eeprom", sh_cmd_eeprom },
#endif
#if (FW_SHELL_U3SEND_ENABLED == TRUE) 
    { "u3send", sh_cmd_uart3_send },
#endif
#if (FW_SHELL_GO2BOOT_ENABLED == TRUE)    
    { "boot", sh_cmd_startboot },
#endif
#if (FW_SHELL_GO2APP1_ENABLED == TRUE)    
    { "app1", sh_cmd_startapp1 },
#endif
    { "reset", sh_cmd_reset },
#if (FW_SHELL_APPCMD_ENABLED == TRUE)
    { "cmd", sh_cmd_appcmd },
#endif    
    { NULL, NULL }
};

#if (FW_SHELL_ON_USB == TRUE)
const ShellConfig shell_cfg_sdu = {(BaseSequentialStream*)&SDU1, commands };
#endif
#if (FW_SHELL_ON_SERIAL3 == TRUE)
const ShellConfig shell_cfg_sd3 = {(BaseSequentialStream*)&SD3, commands };
#endif
#if ((FW_SHELL_ON_SERIAL3 == FALSE) && (FW_SHELL_ON_USB == FALSE))
#error "No shell port configured."
#endif
