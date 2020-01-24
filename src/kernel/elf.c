/**
 * @file elf.c
 * Functionality to execute ELF files
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

#include "elf.h"
#include "vfs.h"
#include "heap.h"

#include <string.h>

/**
 * Finds the section with the given name in the ELF data, e.g. ".bss".
 * @param ehdr ELF file header
 * @param name Name of the section
 * @return Section header of the section, NULL if not found
 */
Elf32_Shdr *elf_find_section(Elf32_Ehdr *ehdr, const char *name)
{
	Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)ehdr + ehdr->e_shoff);
	Elf32_Shdr *shdr_str = (Elf32_Shdr *)((char *)ehdr + ehdr->e_shoff +
		ehdr->e_shstrndx * ehdr->e_shentsize);

	for (Elf32_Half i = 0; i < ehdr->e_shnum; i++) {
		char *section = ((char *)ehdr + shdr_str->sh_offset) +
			shdr->sh_name;
		if (!strcmp(section, name))
			return shdr;

		shdr = (Elf32_Shdr *)((char *)shdr + ehdr->e_shentsize);
	}

	return 0;
}

uint32_t elf_execve(const char *file, char * const argv[], char * const envp[])
{
	(void)argv;
	(void)envp;

	// Open the given file
	int fd = vfs_open(file, VFS_FILE_READ);
	if (fd == -1)
		return -1;

	// Get file size
	vfs_seek(fd, 0, SEEK_END);
	unsigned int fileLength = vfs_tell(fd);
	vfs_seek(fd, 0, SEEK_SET);

	// Read entire file
	char *elfData = malloc(fileLength);
	if (vfs_read(fd, fileLength, (uint8_t *)elfData) != fileLength) {
		vfs_close(fd);
		free(elfData);
		return -1;
	}

	// ELF signature check
	if (elfData[0] != '\177' || elfData[1] != 'E' || elfData[2] != 'L' ||
		elfData[3] != 'F') {
		vfs_close(fd);
		free(elfData);
		return -1;
	}


	uint32_t ELF_OFFSET = 0;

	// Load program headers
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elfData;
	Elf32_Phdr *phdr = (Elf32_Phdr *)(elfData + ehdr->e_phoff);

    // Search for the loadable program header
	for (Elf32_Half i = 0; i < ehdr->e_phnum; i++) {
		if (phdr->p_type == PT_LOAD) {
			// There should only be one PT_LOAD section...
			if (ELF_OFFSET != 0)
				while (1);
			ELF_OFFSET = (uint32_t)malloc(phdr->p_memsz + 8) & ~7;

            // Copy the program data into RAM for execution
			uint8_t *src = (uint8_t *)elfData + phdr->p_offset;
			uint8_t *dst = (uint8_t *)(ELF_OFFSET + phdr->p_vaddr);
			for (uint32_t j = 0; j < phdr->p_filesz; j++)
				dst[j] = src[j];
		}

		phdr = (Elf32_Phdr *)((char *)phdr + ehdr->e_phentsize);
	}

	// Zero the .bss section
	Elf32_Shdr *bssSection = elf_find_section(ehdr, ".bss");
	if (bssSection != 0) {
		uint32_t *bssArray = (Elf32_Addr *)(ELF_OFFSET + bssSection->sh_addr);
		Elf32_Word bssSize = bssSection->sh_size / sizeof(void *);
		for (Elf32_Word i = 0; i < bssSize; i++)
			bssArray[i] = 0;
	}

	// Fix GOT entries if they exist
	Elf32_Shdr *gotSection = elf_find_section(ehdr, ".got");
	if (gotSection != 0) {
		Elf32_Addr *gotArray = (Elf32_Addr *)(ELF_OFFSET + gotSection->sh_addr);
		Elf32_Word gotSize = gotSection->sh_size / sizeof(void *);
		for (Elf32_Word i = 0; i < gotSize; i++)
			gotArray[i] += ELF_OFFSET;
	}

	// Run any initial constructors
	Elf32_Shdr *initArraySection = elf_find_section(ehdr, ".init_array");
	if (initArraySection != 0) {
		Elf32_Addr *initArray = (Elf32_Addr *)(ELF_OFFSET +
			initArraySection->sh_addr);
	
		Elf32_Word initArraySize = initArraySection->sh_size / sizeof(void *);
		for (Elf32_Word i = 0; i < initArraySize; i++) {
			initArray[i] += ELF_OFFSET;
			((void (*)(void))(initArray[i]))();
		}
	}

	vfs_close(fd);
	free(elfData);

	// Return entry point
	return (ELF_OFFSET + ehdr->e_entry);
}

