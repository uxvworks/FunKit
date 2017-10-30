#!/bin/sh

#echo Converting .elf to .s19...
cd build

#arm-none-eabi-objcopy -O srec uxv_mbr.elf uxv_mbr.s19

echo Trying to program device...
st-flash write uxv_mbr.bin 0x8000000
echo Finished flashing device
cd ..

