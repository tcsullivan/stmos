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
	-Iinclude -Iinclude/cmsis \
	-fno-builtin -fsigned-char -ffreestanding \
	-Wall -Werror -Wextra -pedantic \
	-Wno-overlength-strings -Wno-discarded-qualifiers
LFLAGS = -T link.ld

CFILES = $(wildcard src/*.c)
AFILES = $(wildcard src/*.s) 

OUTDIR = out
OFILES = $(patsubst src/%.c, $(OUTDIR)/%.o, $(CFILES)) \
	 $(patsubst src/%.s, $(OUTDIR)/%.asm.o, $(AFILES))

OUT = out/main.elf

all: $(OUT)

$(OUT): $(OFILES)
	@echo "  LINK   " $(OUT)
	@$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) out/*.o -o $(OUT)

$(OUTDIR)/%.o: src/%.c
	@echo "  CC     " $<
	@$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.asm.o: src/%.s
	@echo "  AS     " $<
	@$(CROSS)$(AS) $(AFLAGS) -c $< -o $@

clean:
	@echo "  CLEAN"
	@rm -rf out/*


