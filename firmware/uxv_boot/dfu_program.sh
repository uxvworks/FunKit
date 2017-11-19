#!/bin/sh

cd build

echo Trying to program device...
dfu-util -a 0 --dfuse-address 0x8020000 -D uxv_boot.bin
cd ..

