#!/bin/sh

#echo Converting .elf to .s19...
cd build
#arm-none-eabi-objcopy -O binary bootloader.elf bootloader.bin
#arm-none-eabi-objcopy -O ihex bootloader.elf bootloader.hex
#arm-none-eabi-objcopy -O srec uxv_prog.elf uxv_prog.s19

echo Trying to program device...
st-flash write uxv_prog.bin 0x8040000
echo Finished flashing device
cd ..

