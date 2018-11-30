##
# @file Makefile
# Script to build folder of source files
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

CFILES = $(wildcard *.c)
AFILES = $(wildcard *.s) 
OFILES = $(patsubst %.c, %.o, $(CFILES)) \
	 $(patsubst %.s, %.asm.o, $(AFILES))

CFLAGS += -ffreestanding -nostdlib -I.. -I../arch/cmsis

all: $(OFILES)

%.o: %.c
	@echo "  CC     " $<
	@$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

%.asm.o: %.s
	@echo "  AS     " $<
	@$(CROSS)$(AS) $(AFLAGS) -c $< -o $@

clean:
	@echo "  CLEAN"
	@rm -f $(OFILES)


