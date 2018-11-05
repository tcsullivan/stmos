#include <stdint.h>
#include <string.h>

#include <kernel/heap.h>
#include <kernel/vfs.h>

typedef struct {
	char *address;
	uint32_t pos;
	uint32_t size;
} initrd_info;

extern uint8_t _binary_initrd_img_start[];
extern uint8_t _binary_initrd_img_size[];

static const uint8_t *initrd_start = (uint8_t *)_binary_initrd_img_start;
static const uint32_t initrd_size = (uint32_t)_binary_initrd_img_size;

void *initrd_open(const char *file);
uint32_t initrd_read(void *info, uint32_t count, uint8_t *buffer);

char *initrd_getfile(uint32_t offset);

static const vfs_volume_funcs initrd_funcs = {
	initrd_open,
	0, // close
	initrd_read,
	0, // write
	0  // readdir
};

void initrd_init(void)
{
	vfs_mount(&initrd_funcs, VFS_READONLY);
}

void *initrd_open(const char *file)
{
	char *ptr;
	for (uint32_t i = 0; ptr = initrd_getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(file, ptr + 4, len)) {
			initrd_info *file = (initrd_info *)malloc(
				sizeof(initrd_info));
			file->address = ptr + len + 8;
			file->pos = 0;
			file->size = *(uint32_t *)(ptr + len + 4);
			return file;
		}
	}

	return 0;
}

uint32_t initrd_read(void *info, uint32_t count, uint8_t *buffer)
{
	initrd_info *iinfo = (initrd_info *)info;
	if (iinfo == 0 || iinfo->address == 0)
		return 0;
	
	uint32_t i;
	for (i = 0; i < count; i++) {
		if (iinfo->pos >= iinfo->size)
			break;

		buffer[iinfo->pos] = iinfo->address[iinfo->pos];
		iinfo->pos++;
	}

	return i;
}

/*char *readfile(const char *name)
{
	char *ptr;
	for (uint32_t i = 0; ptr = getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(name, ptr + 4, len))
			return ptr + len + 8;
	}
	return 0;
}*/

char *initrd_getfile(uint32_t offset)
{
	char *ptr = (char *)initrd_start;
	for (uint32_t i = 0; i < offset; i++) {
		uint32_t len = *((uint32_t *)ptr);
		uint32_t datalen = *((uint32_t *)(ptr + 4 + len));
		ptr += len + datalen + 8;
		if (ptr >= (char *)(initrd_start + initrd_size))
			return 0;
	}

	return ptr;
}

