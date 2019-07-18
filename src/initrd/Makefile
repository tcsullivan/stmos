##
# A simple Makefile for building executables loadable by stmos.
#

CC = gcc -mcpu=cortex-m4 -mthumb -fsigned-char
CFLAGS = -Os -fPIE -I../libgpio

CFILES = $(wildcard *.c)
OFILES = $(patsubst %.c, files/%, $(CFILES))

all: $(OFILES)
	@../../tools/rba initrd.img $$(find files/*)
	@$(CROSS)$(OBJCOPY) -B arm -I binary -O elf32-littlearm initrd.img \
		initrd.img.o

files/%: %.c
	@echo "  CC     " $<
	@$(CROSS)$(CC) $(CFLAGS) $< ../libgpio/libgpio.a -o $@
	@cp -v $@ init.debug
	@$(CROSS)strip $@

clean:
	@echo "  CLEAN"
	@rm -f $(OFILES)
	@rm -f initrd.img initrd.img.o

