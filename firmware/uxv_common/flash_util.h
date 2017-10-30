#ifndef _FW_FLASH_UTIL_H
#define _FW_FLASH_UTIL_H

#include "board.h"  
#include "stm32f4xx_hal_flash.h"

#define FLASH_SECTORS 12
#define MBR_BASE 0
#define MBR_SECTORS 2
#define BOOT_BASE 5
#define BOOT_SECTORS 1
#define APP1_BASE 6
#define APP1_SECTORS 3
#define APP2_BASE 9
#define APP2_SECTORS 3
//#define NEW_APP_MAX_SIZE (3 * (1 << 17))

// Base address of the Flash sectors
#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  // Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  // Base @ of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  // Base @ of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  // Base @ of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  // Base @ of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  // Base @ of Sector 5, 128 Kbytes
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  // Base @ of Sector 6, 128 Kbytes
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  // Base @ of Sector 7, 128 Kbytes
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  // Base @ of Sector 8, 128 Kbytes
#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  // Base @ of Sector 9, 128 Kbytes
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000) // Base @ of Sector 10, 128 Kbytes
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000) // Base @ of Sector 11, 128 Kbytes



#define BOOTLOADER_STARTADDR_PTR ((uint32_t)(flash_addr[BOOT_BASE] + 0x00000004))
#define BOOTLOADER_VECTABLE_OFFSET ((uint32_t)flash_addr[BOOT_BASE])

#define PROGRAM_STARTADDR_PTR ((uint32_t)(flash_addr[APP_BASE] + 0x00000004))
#define PROGRAM_VECTABLE_OFFSET ((uint32_t)flash_addr[APP_BASE])


#ifdef __cplusplus
extern "C" {
#endif
extern const uint32_t flash_addr[FLASH_SECTORS];
extern void flash_erase_user_data(BaseSequentialStream* chp);
extern void flash_store_user_data(uint32_t data);
extern uint32_t flash_verify_user_data(uint32_t data);
#ifdef __cplusplus
}
#endif

#endif /* _FW_FLASH_UTIL_H */
