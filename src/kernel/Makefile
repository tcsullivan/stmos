CFILES = $(wildcard *.c)
AFILES = $(wildcard *.s) 
OFILES = $(patsubst %.c, %.o, $(CFILES)) \
	 $(patsubst %.s, %.asm.o, $(AFILES))

CFLAGS += -I.. -I../arch/cmsis

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


