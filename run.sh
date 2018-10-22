#!/bin/bash
#
# @file run.sh
# Starts openocd and connects gdb to the target, for programming/debugging
#

openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg &
sleep 1 # Need to wait some time for openocd to connect
gdb-multiarch -ex "target remote localhost:3333" main.elf
pkill openocd # Ensure openocd exits when we're done
