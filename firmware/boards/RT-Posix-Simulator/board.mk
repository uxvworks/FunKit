# List of all the simulator board related files.
BOARDSRC = $(BOARDDIR)/RT-Posix-Simulator/board.c

# Required include directories
BOARDINC = $(BOARDDIR)/RT-Posix-Simulator

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
