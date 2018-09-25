/**
 * @file linker.ld
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* enter at the reset handler */
ENTRY(Reset_Handler)

/* description of memory regions */
MEMORY {
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
	RAM (rwx)  : ORIGIN = 0x20000000,  LENGTH = 96K
}

/* description of ELF sections */
SECTIONS {
	/* keep startup code at beginning */
	.isr_vector : {
		. = ALIGN(8);
		KEEP(*(.isr_vector))
		. = ALIGN(8);
	} > FLASH

	/* other code sections */
	.text : {
		. = ALIGN(8);
		*(.text)
		*(.text*)

		KEEP(*(.init))
		KEEP(*(.fini))
		. = ALIGN(8);
	} > FLASH

	/* read-only data sections */
	.rodata : {
		. = ALIGN(8);
		*(.rodata)
		*(.rodata*)

		*(.eh_frame)
		. = ALIGN(8);
	} > FLASH

	/* ARM stuff */
	.ARM.exidx : {
		*(.ARM.exidx)
	} > FLASH

	/* init_array/fini_array (TODO understand this) */
	.init_array : {
		__init_array_start = .;
		KEEP(*(.init_array.*))
		KEEP(*(.init_array*))
		__init_array_end = .;
	} > FLASH

	.fini_array : {
		__fini_array_start = .;
		KEEP(*(.fini_array.*))
		KEEP(*(.fini_array*))
		__fini_array_end = .;
	} > FLASH

	/* initialized data */
	_sidata = LOADADDR(.data);
	.data : {
		. = ALIGN(8);
		_sdata = .;
		*(.data)
		. = ALIGN(8);
		_edata = .;
	} > RAM AT > FLASH

	/* uninitialized data */
	.bss : {
		. = ALIGN(8);
		__bss_start__ = .;
		*(.bss)
		. = ALIGN(8);
		__bss_end__ = .;
	} > RAM
}
