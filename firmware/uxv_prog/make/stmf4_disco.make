##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -std=gnu++11 -fno-exceptions -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data.
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT = 
endif

# Enable this if you want link time optimizations (LTO).
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x400
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

# Enables the use of FPU (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = hard
endif

# FPU-related options.
ifeq ($(USE_FPU_OPT),)
  USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv4-sp-d16
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, target, sources and paths
#

# Define project name here
PROJECT = uxv_prog

# Target settings.
MCU  = cortex-m4

# Imported source files and paths.
CHIBIOS  := ../chibios_19.1.3
BOARDDIR := ../boards
CONFDIR  := ./cfg/stmf4_disco
BUILDDIR := ./build/stmf4_disco
DEPDIR   := ./.dep/stmf4_disco

STCMSISINC = ../stm/STM32Cube_FW_F4_V1.17.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include
STM32INC = ../stm/STM32Cube_FW_F4_V1.17.0/Drivers/STM32F4xx_HAL_Driver/Inc
STM32SRC = ../stm/STM32Cube_FW_F4_V1.17.0/Drivers/STM32F4xx_HAL_Driver/Src
UXV_COMMON = ../uxv_common
UXV_DEVICE = ../uxv_device

# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include $(BOARDDIR)/ST_STM32F4_DISCOVERY/board.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
# Auto-build files in ./source recursively.
include $(CHIBIOS)/tools/mk/autobuild.mk
# Other files (optional).
include $(CHIBIOS)/test/lib/test.mk
include $(CHIBIOS)/test/rt/rt_test.mk
include $(CHIBIOS)/test/oslib/oslib_test.mk

include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/shell/shell.mk
#include $(CHIBIOS)/os/various/lwip_bindings/lwip.mk
#include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk

include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk

# Define linker script file here
LDSCRIPT= ./make/STM32F405xG_app1.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(ALLCSRC) \
       $(TESTSRC) \
       $(CHIBIOS)/os/various/evtimer.c \
       $(CONFDIR)/portab.c \
       $(STM32SRC)/stm32f4xx_hal_flash.c \
       $(STM32SRC)/stm32f4xx_hal_flash_ex.c \
       $(UXV_COMMON)/flash_util.c \
       $(UXV_COMMON)/cmd_func.c \
       $(UXV_COMMON)/uart_func.c \
       $(UXV_COMMON)/shell_func.c \
       $(UXV_COMMON)/stm_eeprom.c \
       $(UXV_COMMON)/thread_func.c \
       $(UXV_COMMON)/cfg_storage.c \
       $(UXV_DEVICE)/ws2812/ws2812.c \
       $(UXV_DEVICE)/sdcard/sdcard_mgr.c \
#       main.c

#       $(UXV_COMMON)/usbcfg.c \
#       $(DWAPI)/decadriver/deca_range_tables.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC) \
         main.cpp 

#        $(CHIBIOS)/os/various/cpp_wrappers/syscalls_cpp.cpp \

# List ASM source files here.
ASMSRC = $(ALLASMSRC)

# List ASM with preprocessor source files here.
ASMXSRC = $(ALLXASMSRC) \
          $(OSALASM)
#          $(STARTUPASM)
#          $(PORTASM)

# Inclusion directories.
INCDIR = $(CONFDIR) $(ALLINC) $(TESTINC) ./cfg \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(STREAMSINC) $(SHELLINC) \
         $(CHCPPINC) $(CHIBIOS)/os/various


#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#
#TRGT = arm-elf-
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
# LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary
SREC  = $(CP) -O srec


# Define C warning options here.
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# Define C++ warning options here.
CPPWARN = -Wall -Wextra -Wundef

#
# Project, target, sources and paths
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS = -DSHELL_CONFIG_FILE -DUSE_RTOS=0 

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR = $(CONFDIR) \
          $(STM32INC) \
          $(STCMSISINC) \
          $(UXV_COMMON) \
          $(UXV_DEVICE)

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = -lm

#
# End of user section
##############################################################################

##############################################################################
# Common rules
#

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
include $(RULESPATH)/arm-none-eabi.mk
include $(RULESPATH)/rules.mk

#
# Common rules
##############################################################################

##############################################################################
# Custom rules
#

#
# Custom rules
##############################################################################
