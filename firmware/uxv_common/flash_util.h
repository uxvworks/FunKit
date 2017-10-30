#ifndef _FW_FLASH_UTIL_H
#define _FW_FLASH_UTIL_H





#ifdef __cplusplus
extern "C" {
#endif

extern void flash_erase_user_data(void);
extern void flash_store_user_data(uint32_t data);
extern uint32_t flash_verify_user_data(uint32_t data);
#ifdef __cplusplus
}
#endif

#endif /* _FW_FLASH_UTIL_H */
