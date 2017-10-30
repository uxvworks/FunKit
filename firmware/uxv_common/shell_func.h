#ifndef _FW_SHELL_FUNC_H
#define _FW_SHELL_FUNC_H

#include "shellconf.h"

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)


#ifdef __cplusplus
extern "C" {
#endif

#if (FW_SHELL_ON_USB == TRUE)
extern const ShellConfig shell_cfg_sdu;
#endif
#if (FW_SHELL_ON_SERIAL3 == TRUE)
extern const ShellConfig shell_cfg_sd3;
#endif


#ifdef __cplusplus
}
#endif

#endif /* _FW_SHELL_FUNC_H */
