
#include <string.h>

#include "cfg_storage.h"
#include "stm_eeprom.h"

#include "board.h"
#include "stm32f4xx_hal_flash.h"


#define EEPROM_BASE_APPCFG		0x10

uint16_t VirtAddVarTab[EE_NUM_VARS];

void cfg_storage_init(void)
{
    memset(VirtAddVarTab, 0, sizeof(VirtAddVarTab));

    // the EEPROM is accessed as 16 bit half words...
    if (EE_NUM_VARS < (sizeof(app_cfg_t)/2)) {
         ///#error  "EEPROM NUM_VARS configured too small for app_cfg storage!"
    }

    uint32_t index = 0;

    for (uint32_t i = 0; i < (sizeof(app_cfg_t) / 2); i++) {
        VirtAddVarTab[index++] = EEPROM_BASE_APPCFG + i;
    }

    HAL_FLASH_Unlock();
    //FLASH_ClearFlag(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
    //                FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    EE_Init();
}


void cfg_storage_default(app_cfg_t *cfg)
{
    memset(cfg, 0, sizeof(app_cfg_t));

    cfg->item32A = 23;
    cfg->item32B = 17;
    strcpy((char*)cfg->item8, "uxv_works");
}



void cfg_storage_read(app_cfg_t *cfg)
{
    uint8_t *cfg_addr = (uint8_t*)cfg;
    uint16_t var;
    bool read_error = false;

    for (uint32_t i = 0; i < (sizeof(app_cfg_t) / 2); i++) {
        if (EE_ReadVariable(EEPROM_BASE_APPCFG + i, &var) == EE_OK) {
            cfg_addr[2 * i] = (var >> 8) & 0xFF;
            cfg_addr[2 * i + 1] = var & 0xFF;
        } else {
            read_error = true;
            break;
        }
    }

    // revert to default configuration
    if (read_error) {
        cfg_storage_default(cfg);
    }
}


bool cfg_storage_write(app_cfg_t *cfg)
{

    bool is_ok = true;
    uint8_t *cfg_addr = (uint8_t*)cfg;
    uint16_t var;

    HAL_FLASH_Unlock();

    //FLASH_ClearFlag(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
    //                FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    for (uint32_t i = 0; i < (sizeof(app_cfg_t) / 2); i++) {
        var = (cfg_addr[2 * i] << 8) & 0xFF00;
        var |= cfg_addr[2 * i + 1] & 0xFF;

        if (EE_WriteVariable(EEPROM_BASE_APPCFG + i, var) != EE_OK) {
            is_ok = false;
            break;
        }
    }

    return is_ok;
}
