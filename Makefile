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

CROSS = arm-none-eabi-
CC = gcc
AS = as

MCUFLAGS = -mthumb -mcpu=cortex-m4 #-mfloat-abi=hard -mfpu=fpv4-sp-d16
AFLAGS = $(MCUFLAGS) 
CFLAGS = $(MCUFLAGS) -ggdb \
	-I.. \
	-fno-builtin -fsigned-char -ffreestanding \
	-Wall -Werror -Wextra -pedantic \
	-Wno-overlength-strings -Wno-discarded-qualifiers
LFLAGS = -T link.ld

OUT = main.elf

export

all: 
	@$(MAKE) -C src/kernel
	@$(MAKE) -C src/user
	@echo "  LINK   " $(OUT)
	@$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) -o $(OUT) $$(find src/ -name "*.o")


clean:
	@echo "  CLEAN"
	@$(MAKE) -C src/kernel clean
	@$(MAKE) -C src/user clean
	@rm -f $(OUT)

