#ifndef _CFG_STORAGE_H_
#define _CFG_STORAGE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>



#ifdef __cplusplus
extern "C" {
#endif


typedef struct {

    uint32_t item32A;
    uint32_t item32B;

    uint8_t  item8[10];
   // uint8_t  item8B;
   // uint8_t  itemcc;
   // char     item232;
} app_cfg_t;

// the EEPROM is accessed as 16 bit half words...
#define EE_NUM_VARS ((sizeof(app_cfg_t)/2))

extern uint16_t VirtAddVarTab[EE_NUM_VARS];

void cfg_storage_init(void);
void cfg_storage_default(app_cfg_t *cfg);
void cfg_storage_read(app_cfg_t *cfg);
bool cfg_storage_write(app_cfg_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* _CFG_STORAGE_H_ */
