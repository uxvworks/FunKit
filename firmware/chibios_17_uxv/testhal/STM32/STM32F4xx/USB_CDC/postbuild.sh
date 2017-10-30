#!/bin/sh

#echo Converting .elf to .bin...
#cd build
#arm-none-eabi-objcopy -O binary bootloader.elf bootloader.bin
#arm-none-eabi-objcopy -O ihex bootloader.elf bootloader.hex
#arm-none-eabi-objcopy -O srec bootloader.elf bootloader.s19

echo Trying to program device...
st-flash write build/ch.bin 0x8000000
echo Finished flashing device
#cd ..

