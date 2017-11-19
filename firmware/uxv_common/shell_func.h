#ifndef _FW_SHELL_FUNC_H
#define _FW_SHELL_FUNC_H

#include "shellconf.h"

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)


#ifdef __cplusplus
extern "C" {
#endif

#define streq(s1, s2)   (strcmp((s1), (s2)) == 0)

#if (FW_SHELL_APPCMD_ENABLED == TRUE)
    extern void sh_cmd_appcmd(BaseSequentialStream* chp, int argc, char* argv[]);
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
