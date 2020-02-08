##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32F407-Olimex ==================================
	+@make --no-print-directory -f make/olimex_e407.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F4-Discovery ===============================
	+@make --no-print-directory -f make/stmf4_disco.make all
	@echo ====================================================================
	@echo
	@echo === Building for MIKRO-Clicker2 ===============================
	+@make --no-print-directory -f make/mikro_clicker2.make all
	@echo ====================================================================
	@echo
#	@echo === Building for POSIX-Simulator ===============================
#	+@make --no-print-directory -f make/posix_sim.make all
#	@echo ====================================================================
#	@echo
#	@echo === Building for STM32-Clicker ===============================
#	+@make --no-print-directory -f make/stm_clicker.make all
#	@echo ====================================================================
#	@echo
#	@echo === Building for STM32F107-GoldBull ================================
#	+@make --no-print-directory -f make/stm32f107_goldbull.make all
#	@echo ====================================================================
#	@echo

clean:
	@echo
	+@make --no-print-directory -f make/olimex_e407.make clean
	@echo
	+@make --no-print-directory -f make/stmf4_disco.make clean
	@echo
	+@make --no-print-directory -f make/mikro_clicker2.make clean
	@echo
#	+@make --no-print-directory -f make/posix_sim.make clean
#	@echo
#	+@make --no-print-directory -f make/stm_clicker.make clean
#	@echo
#	+@make --no-print-directory -f make/stm32f107_goldbull.make clean
#	@echo
#
##############################################################################
