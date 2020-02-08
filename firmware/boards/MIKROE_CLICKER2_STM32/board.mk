# List of all the board related files.
BOARDSRC = $(BOARDDIR)/MIKROE_CLICKER2_STM32/board.c

# Required include directories
BOARDINC = $(BOARDDIR)/MIKROE_CLICKER2_STM32

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
