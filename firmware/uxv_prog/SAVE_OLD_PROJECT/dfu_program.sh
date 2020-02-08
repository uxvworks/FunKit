#!/bin/sh

cd build

echo Trying to program device...
dfu-util -a 0 --dfuse-address 0x8040000 -D uxv_prog.bin
cd ..

