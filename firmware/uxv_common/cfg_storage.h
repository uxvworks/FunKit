#ifndef _CFG_STORAGE_H_
#define _CFG_STORAGE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "stm_eeprom.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {

    uint32_t item32A;
    uint32_t item32B;

    uint8_t  item8[10];
    uint8_t  item8B;
    uint8_t  itemcc;
    char     item232;
} app_cfg_t;


// IMPORTANT - this needs to be sized big enough for the app_cfg storage 
//  256 equals 512 bytes of configuration storage
#define EE_NUM_VARS (256)    // number of (16 bit) virtual storage addresses

// Virtual address defined by the user: 0xFFFF value is prohibited 
extern uint16_t VirtAddVarTab[EE_NUM_VARS];

void cfg_storage_init(void);
void cfg_storage_default(app_cfg_t *cfg);
void cfg_storage_read(app_cfg_t *cfg);
bool cfg_storage_write(app_cfg_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* _CFG_STORAGE_H_ */
