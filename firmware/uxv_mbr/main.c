
#include <stdint.h>

#include "hal.h"

#include "flash_defines.h"
#include "flash_util.h"
#include "cmd_func.h"



int main(void)
{
    halInit();
//    palSetPadMode(GPIOC, PIN_BUT1, PAL_MODE_INPUT_PULLUP);
    
    //osalSysEnable();

    // say hello
    palSetLine(LINE_LED1);
    palClearLine(LINE_LED2);

    //osalThreadSleepMilliseconds(500);

    if(palReadLine(LINE_BUT1) == LINE_BUT1_PRESSED) {
        cmd_func_goto_exec(BOOT_BASE_ADDR); //cmd_func_goto_sysboot(); cmd_func_reset(void); 
    } else {
        cmd_func_goto_exec(APP1_BASE_ADDR);
    }
    
    return 0;
}
