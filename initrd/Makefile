##
# A simple Makefile for building executables loadable by stmos.
#

ARCH = arm-stmos-
CC = gcc -mcpu=cortex-m4 -mthumb -fsigned-char
CFLAGS = -Os -fPIE

all:
	@$(ARCH)$(CC) $(CFLAGS) init.c -o init
	@arm-stmos-strip init

