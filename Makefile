##
# @file Makefile
# Script to build source files
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

CROSS = arm-stmos-
CC = gcc
AS = as
OBJCOPY = objcopy

MCUFLAGS = -mthumb -mcpu=cortex-m4 #-mfloat-abi=hard -mfpu=fpv4-sp-d16
AFLAGS = $(MCUFLAGS) -meabi=5
CFLAGS = $(MCUFLAGS) -ggdb -fsigned-char -I.. \
	-Wall -Werror -Wextra -pedantic
LFLAGS = -T link.ld 

OUT = main.elf

export

all: 
	@$(MAKE) -C src/kernel
	@$(MAKE) -C src/fs
	@$(MAKE) -C src/user
	@$(MAKE) -C src/initrd
	@echo "  LINK   " $(OUT)
	@$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o $(OUT) \
		$$(find src/fs src/kernel src/user src/initrd -name "*.o")

crt:
	@arm-stmos-$(CC) $(MCUFLAGS) -fsigned-char -Os -fPIE -c src/crt/crt0.c \
		-o src/crt/crt0.o

initrd:
	@$(MAKE) -C src/initrd

libgpio:
	@$(MAKE) -C src/libgpio

clean:
	@echo "  CLEAN"
	@$(MAKE) -C src/kernel clean
	@$(MAKE) -C src/fs clean
	@$(MAKE) -C src/user clean
	@$(MAKE) -C src/initrd clean
	@rm -f $(OUT)
	@rm -f initrd.img initrd.img.o

