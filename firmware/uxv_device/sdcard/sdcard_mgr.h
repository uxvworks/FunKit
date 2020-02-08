#ifndef _SDCARD_MGR_H_
#define _SDCARD_MGR_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "ch.h"

#ifdef __cplusplus
extern "C" {
#endif

extern event_source_t sdcard_inserted_event;
extern event_source_t sdcard_removed_event;

extern void sdcard_InsertHandler(eventid_t id);
extern void sdcard_RemoveHandler(eventid_t id);
extern void sdcard_mgr_start(void);

#ifdef __cplusplus
}
#endif

#endif /* _SDCARD_MGR_H_ */
