# List of all the board related files.
BOARDSRC = $(BOARDDIR)/ST_STM32F4_DISCOVERY/board.c

# Required include directories
BOARDINC = $(BOARDDIR)/ST_STM32F4_DISCOVERY

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
