
#include <stdint.h>
#include "hal.h"
#include "flash_util.h"
#include "board.h"  
#include "stm32f4xx_hal_flash.h"
#include "flash_defines.h"


void HAL_Delay(__IO uint32_t Delay)
{
    osalThreadSleepMilliseconds(Delay);
}

uint32_t HAL_GetTick(void)
{
    return (osalOsGetSystemTimeX());   //OSAL_ST2MS(osalOsGetSystemTimeX()));
}



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;
__IO uint32_t data32 = 0;
uint32_t MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
//static void Error_Handler(void);
static uint32_t GetSector(uint32_t Address);


void flash_erase_user_data(void)
{
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Get the 1st sector to erase */
    FirstSector = GetSector(FLASH_USER_START_ADDR);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        /*
          Error occurred while sector erase.
          User can add here some code to deal with this error.
          SectorError will contain the faulty sector and then to know the code error on this sector,
          user can call function 'HAL_FLASH_GetError()'
        */
        /*
          FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
        */
        //palSetLine(LINE_LED2); 
        //Error_Handler();
    }


    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();


    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    //HAL_FLASH_Lock();
}


/* Program the user Flash area word by word
   (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
void flash_store_user_data(uint32_t user_data)
{
    Address = FLASH_USER_START_ADDR;

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    while (Address < FLASH_USER_END_ADDR) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, user_data) == HAL_OK) {
            Address = Address + 4;
        } else {
            /* Error occurred while writing data in Flash memory.
               User can add here some code to deal with this error */
            /*
              FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
            */
            ; //palSetLine(LINE_LED2); //Error_Handler();
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
    to protect the FLASH memory against possible unwanted operation) *********/
    //HAL_FLASH_Lock();
}



uint32_t flash_verify_user_data(uint32_t user_data)
{
    /* Check if the programmed data is OK
         MemoryProgramStatus = 0: data programmed correctly
         MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = 0x0;

    while (Address < FLASH_USER_END_ADDR) {
        data32 = *(__IO uint32_t*)Address;

        if (data32 != user_data) {
            MemoryProgramStatus++;
        }

        Address = Address + 4;
    }

    /* Check if there is an issue to program data */
    if (MemoryProgramStatus == 0) {
        ; /* No error detected. Switch on LED4 */
        //BSP_LED_On(LED4);
    } else {
        ; /* Error detected. Switch on LED5 */
        //palSetLine(LINE_LED2); //Error_Handler();
    }

    return (MemoryProgramStatus);
}


/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
        sector = FLASH_SECTOR_0;
    } else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) {
        sector = FLASH_SECTOR_1;
    } else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)) {
        sector = FLASH_SECTOR_2;
    } else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)) {
        sector = FLASH_SECTOR_3;
    } else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)) {
        sector = FLASH_SECTOR_4;
    } else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)) {
        sector = FLASH_SECTOR_5;
    } else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)) {
        sector = FLASH_SECTOR_6;
    } else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7)) {
        sector = FLASH_SECTOR_7;
    } else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8)) {
        sector = FLASH_SECTOR_8;
    } else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9)) {
        sector = FLASH_SECTOR_9;
    } else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)) {
        sector = FLASH_SECTOR_10;
    } else { /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
        sector = FLASH_SECTOR_11;
    }

    return sector;
}

