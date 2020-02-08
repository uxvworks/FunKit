
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "portab.h"
#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "sdcard_mgr.h"


#if (FW_INCLUDE_SDCARD_MGR == TRUE)
/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

/**
 * @brief   Card monitor timer.
 */
static virtual_timer_t tmr;

/**
 * @brief   Debounce counter.
 */
static unsigned cnt;

/**
 * @brief   Card event sources.
 */
event_source_t sdcard_inserted_event, sdcard_removed_event;

/**
 * @brief   Insertion monitor timer callback function.
 *
 * @param[in] p         pointer to the @p BaseBlockDevice object
 *
 * @notapi
 */
static void tmrfunc(void *p) {
  BaseBlockDevice *bbdp = (BaseBlockDevice *)p;

  chSysLockFromISR();
  if (cnt > 0) {
    if (blkIsInserted(bbdp)) {
      if (--cnt == 0) {
        chEvtBroadcastI(&sdcard_inserted_event);
      }
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (!blkIsInserted(bbdp)) {
      cnt = POLLING_INTERVAL;
      chEvtBroadcastI(&sdcard_removed_event);
    }
  }
  chVTSetI(&tmr, TIME_MS2I(POLLING_DELAY), tmrfunc, bbdp);
  chSysUnlockFromISR();
}
#endif


/**
 * @brief   Polling monitor start.
 *
 * @param[in] p         pointer to an object implementing @p BaseBlockDevice
 *
 * @notapi
 */
#if (FW_INCLUDE_SDCARD_MGR == TRUE)
static void tmr_init(void *p) {

  chEvtObjectInit(&sdcard_inserted_event);
  chEvtObjectInit(&sdcard_removed_event);
  chSysLock();
  cnt = POLLING_INTERVAL;
  chVTSetI(&tmr, TIME_MS2I(POLLING_DELAY), tmrfunc, p);
  chSysUnlock();
}
#endif


#if (FW_INCLUDE_SDCARD_MGR == TRUE)
/*===========================================================================*/
/* FatFs related.                                                            */
/*===========================================================================*/

/**
 * @brief FS object.
 */
static FATFS SDC_FS;

/* FS mounted and ready.*/
static bool fs_ready = FALSE;

/* Generic large buffer.*/
static uint8_t fbuff[1024];

static FRESULT scan_files(BaseSequentialStream *chp, char *path) {
  static FILINFO fno;
  FRESULT res;
  DIR dir;
  size_t i;
  char *fn;

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    while (((res = f_readdir(&dir, &fno)) == FR_OK) && fno.fname[0]) {
      if (FF_FS_RPATH && fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        *(path + i) = '/';
        strcpy(path + i + 1, fn);
        res = scan_files(chp, path);
        *(path + i) = '\0';
        if (res != FR_OK)
          break;
      }
      else {
        chprintf(chp, "%s/%s\r\n", path, fn);
      }
    }
  }
  return res;
}
#endif


/*
 * Card insertion event.
 */
void sdcard_InsertHandler(eventid_t id) {
   (void)id;
#if (FW_INCLUDE_SDCARD_MGR == TRUE)
   FRESULT err;
  /*
   * On insertion SDC initialization and FS mount.
   */
#if HAL_USE_SDC
  if (sdcConnect(&SDCD1))
#else
  if (mmcConnect(&MMCD1))
#endif
    return;

  err = f_mount(&SDC_FS, "/", 1);
  if (err != FR_OK) {
#if HAL_USE_SDC
    sdcDisconnect(&SDCD1);
#else
    mmcDisconnect(&MMCD1);
#endif
    return;
  }
  fs_ready = TRUE;
#endif
}

/*
 * Card removal event.
 */
void sdcard_RemoveHandler(eventid_t id) 
{
  (void)id;
#if (FW_INCLUDE_SDCARD_MGR == TRUE)
#if HAL_USE_SDC
    sdcDisconnect(&SDCD1);
#else
    mmcDisconnect(&MMCD1);
#endif
  fs_ready = FALSE;
#endif
}

#if (FW_INCLUDE_SDCARD_MGR == TRUE)
static void cmd_tree(BaseSequentialStream *chp, int argc, char *argv[]) 
{
  FRESULT err;
  uint32_t fre_clust;
  FATFS *fsp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: tree\r\n");
    return;
  }
  if (!fs_ready) {
    chprintf(chp, "File System not mounted\r\n");
    return;
  }
  err = f_getfree("/", &fre_clust, &fsp);
  if (err != FR_OK) {
    chprintf(chp, "FS: f_getfree() failed\r\n");
    return;
  }
  chprintf(chp,
           "FS: %lu free clusters with %lu sectors (%lu bytes) per cluster\r\n",
           fre_clust, (uint32_t)fsp->csize, (uint32_t)fsp->csize * 512);
  fbuff[0] = 0;
  scan_files(chp, (char *)fbuff);
}
#endif


#if (FW_INCLUDE_SDCARD_MGR == TRUE)
void sdcard_mgr_start(void)
{
#if HAL_USE_SDC
  /*
   * Activates the  SDC driver 1 using default configuration.
   */

  sdcStart(&SDCD1, NULL);

  /*
   * Activates the card insertion monitor.
   */
  tmr_init(&SDCD1);
#else
  /*
   * Initializes the MMC driver to work with SPI3.
   */
  palSetPad(IOPORT3, GPIOC_SPI3_SD_CS);
  mmcObjectInit(&MMCD1);
  mmcStart(&MMCD1, &portab_mmccfg);

  /*
   * Activates the card insertion monitor.
   */
  tmr_init(&MMCD1);
#endif
}
#endif


#if 0
static const ShellCommand commands[] = {
  {"tree", cmd_tree},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&PORTAB_SDU1,
  commands
};
#endif



