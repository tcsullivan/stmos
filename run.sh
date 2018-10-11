#!/bin/bash
#
# @file run.sh
# Starts openocd and connects gdb to the target, for programming/debugging
#
# Copyright (C) 2018 Clyne Sullivan
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg &
sleep 1
gdb-multiarch -ex "target remote localhost:3333" main.elf
pkill openocd
