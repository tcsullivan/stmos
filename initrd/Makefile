all:
	@arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -fsigned-char -Os \
		-nostdinc -nostdlib \
		-fdata-sections -ffunction-sections -Wl,--gc-sections \
		-I../src/pdclib/include -I../src/pdclib/platform/stmos/include \
		test.c ../src/pdclib/pdclib.a -s -o test
