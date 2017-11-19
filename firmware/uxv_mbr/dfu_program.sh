#!/bin/sh

cd build

echo Trying to program device...
dfu-util -a 0 --dfuse-address 0x08000000 -D uxv_mbr.bin
cd ..

