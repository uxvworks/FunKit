
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "usbcfg.h"
#include "cmd_func.h"
#include "flash_util.h"


typedef struct {
    uint32_t data[3];
} UID_TypeDef;


//#define UID_BASE              ((uint32_t)0x1FFF7A10U)       /*!< Unique device ID register base address */
#define UID                   ((UID_TypeDef *) UID_BASE)


//typedef struct
//{
//    uint32_t data[8];
//} OTP_TypeDef;

typedef struct {
    uint32_t serial32;
    uint32_t device;
    uint32_t id16;
    uint32_t id8;
    uint8_t  name[16];
} OTP0_TypeDef;


typedef struct {
    uint8_t lock[16];
} OTP_LOCK_TypeDef;


#define OTP0_BASE             ((uint32_t)0x1FFF7800U)       /*!< OTP0 base address */
#define OTP0                  ((OTP0_TypeDef *) OTP0_BASE)
#define OTP_LOCK_BASE         ((uint8_t)0x1FFF7A00U)       /*!< OTP LOCK base address */
#define OTP_LOCK              ((OTP_LOCK_TypeDef *) OTP_LOCK_BASE)


uint8_t cmd_func_otp_get_lock(int index)
{
    return OTP_LOCK->lock[index];
}

void cmd_func_otp_set_lock0(void /* int index*/)
{
    OTP_LOCK->lock[0/*index*/] = 0x00;
}

uint32_t cmd_func_otp_get_serial(void)
{
    return OTP0->serial32;
}

void cmd_func_otp_set_serial(uint32_t serial)
{
    OTP0->serial32 = serial;
}

uint32_t cmd_func_otp_get_device(void)
{
    return OTP0->device;
}

void cmd_func_otp_set_device(uint32_t device)
{
    OTP0->device = device;
}

uint32_t cmd_func_otp_get_id16(void)
{
    return OTP0->id16;
}

void cmd_func_otp_set_id16(uint32_t id16)
{
    OTP0->id16 = id16;
}

uint32_t cmd_func_otp_get_id8(void)
{
    return OTP0->id8;
}

void cmd_func_otp_set_id8(uint32_t id8)
{
    OTP0->id8 = id8;
}

uint8_t* cmd_func_otp_get_name(void)
{
    if (OTP0->name[0] == 0xFF)
        return ((uint8_t*)"empty");
    else
        return OTP0->name;
}

//void cmd_func_uid(uint32_t* uid0, uint32_t* uid1, uint32_t* uid2)
void cmd_func_uid(uint32_t* uid)
{
    // *uid0 = UID->data[0];
    // *uid1 = UID->data[1];
    // *uid2 = UID->data[2];
    uid[0] = UID->data[0];
    uid[1] = UID->data[1];
    uid[2] = UID->data[2];

    //HAL_GetUID(uid);
}

void cmd_func_reset(void)
{
#if (HAL_USE_SERIAL_USB == true)
    usbDisconnectBus(serusbcfg.usbp);
    usbStop(&USBD1);
#endif

    //chSysDisable();
    osalSysDisable();

    palClearLine(LINE_LED1);
    palClearLine(LINE_LED2);

    NVIC_SystemReset();
}

void cmd_func_goto_exec(uint32_t base_sector)
{
    static void (*pResetHandler)(void);

    // TODO first check if app is OK by verifying the checksum */
#if (HAL_USE_SERIAL_USB == true)
    usbDisconnectBus(serusbcfg.usbp);
    usbStop(&USBD1);
#endif

    //chSysDisable();
    osalSysDisable();

    palClearLine(LINE_LED1);
    palClearLine(LINE_LED2);

    // set the vector table offset
    // SCB_VTOR = CPU_USER_PROGRAM_VECTABLE_OFFSET & (uint32_t)0x1FFFFF80;
    // SCB->VTOR = flash_addr[APP_BASE];
    SCB->VTOR = flash_addr[base_sector];

    // pResetHandler = (void (*)(void))(*((uint32_t*)PROGRAM_STARTADDR_PTR));
    pResetHandler = (void (*)(void))(*((uint32_t*)((uint32_t)(flash_addr[base_sector] + 0x00000004))));

    // Set stack pointer ?
    //__set_MSP((uint32_t) (flash_addr[base_sector]));

    /* start the program  */
    pResetHandler();
}
