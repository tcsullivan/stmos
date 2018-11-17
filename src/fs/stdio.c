#include "stdio.h"

#include <kernel/heap.h>
#include <kernel/serial.h>

void *stdio_open(const char *path);
int stdio_close(void *info);
uint32_t stdio_read(void *info, uint32_t count, uint8_t *buffer);
uint32_t stdio_write(void *info, uint32_t count, const uint8_t *buffer);

const vfs_volume_funcs stdio_funcs = {
	stdio_open,
	stdio_close,
	stdio_read,
	stdio_write,
	0, // readdir
};

void *stdio_open(const char *path)
{
	int *id = malloc(sizeof(uint32_t));

	if (path[0] == 'o' && path[1] == 'u' && path[2] == 't')
		*id = 1;
	else if (path[0] == 'i' && path[1] == 'n')
		*id = 0;
	if (path[0] == 'e' && path[1] == 'r' && path[2] == 'r')
		*id = 2;

	return id;
}

int stdio_close(void *info)
{
	// Nothing to do
	free(info);
	return 0;
}

uint32_t stdio_read(void *info, uint32_t count, uint8_t *buffer)
{
	(void)info;
	(void)count;
	(void)buffer;
	return 0;
}

uint32_t stdio_write(void *info, uint32_t count, const uint8_t *buffer)
{
	(void)info;
	for (uint32_t i = 0; i < count; i++)
		serial_put(buffer[count]);
	return count;
}

