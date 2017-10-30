#!/bin/sh

#echo Converting .elf to .s19...
cd build
#arm-none-eabi-objcopy -O binary bootloader.elf bootloader.bin
#arm-none-eabi-objcopy -O ihex bootloader.elf bootloader.hex
#arm-none-eabi-objcopy -O srec uxv_boot.elf uxv_boot.s19

echo Trying to program device...
st-flash write uxv_boot.bin 0x8020000
echo Finished flashing device
cd ..

