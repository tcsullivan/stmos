/**
 * @file vfs.h
 * An implementation of filesystem abstraction
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

#ifndef VFS_H_
#define VFS_H_

#include <stdint.h>

/**
 * Holds file info that can be used by filesystem drivers/modules.
 */
typedef struct {
	uint32_t pos;   /**< Position within the file */
	void *fsinfo;   /**< Filesystem-specific data, handled by fs driver */
} vfs_file_info;

/**
 * Directory entry data, used by readdir.
 */
typedef struct {
	char name[32];
} vfs_dirent;

/**
 * Functions that a filesystem driver/module should implement.
 */
typedef struct vfs_volume_funcs_t {
	/**
	 * Opens the given file.
	 * @param file Path to the file
	 * @return 0 on fail, otherwise pointer to be stored in
	 *         vfs_file_info.fsinfo
	 */
	void *(*open)(const char *file);

	/**
	 * Closes the file, releasing given info however necessary.
	 * @param info File info
	 * @return 0 on success, non-zero on failure
	 */
	int (*close)(vfs_file_info *info);

	/**
	 * Reads bytes from the given file.
	 * @param info Info on the file
	 * @param count Number of bytes to read
	 * @param buffer Pre-allocated buffer to store data to
	 * @return Number of bytes read
	 */
	uint32_t (*read)(vfs_file_info *info, uint32_t count, uint8_t *buffer);

	/**
	 * Writes bytes from the given file.
	 * @param info Info on the file
	 * @param count Number of bytes to write
	 * @param buffer Data to write to file
	 * @return Number of bytes written
	 */
	uint32_t (*write)(vfs_file_info *info, uint32_t count, const uint8_t *buffer);

	/**
	 * Creates an array of vfs_dirent for the given directory.
	 * @param path Directory to read
	 * @return 0 on fail, an array of vfs_dirent otherwise
	 */
	vfs_dirent *(*readdir)(const char *path);

	/**
	 * Seeks file to the given position.
	 * @param info The file's info
	 * @param offset Offset to seek to
	 * @param whence Where to seek from (beginning, end, or current pos.)
	 * @return 0 on succes, non-zero otherwise
	 */
	int (*seek)(vfs_file_info *info, uint32_t offset, int whence);
} vfs_volume_funcs;

// Indicates mounted volume
#define VFS_MOUNTED  (1 << 0)
// Set if filesystem is read-only
#define VFS_READONLY (1 << 1)

/**
 * Data defining a VFS volume.
 */
typedef struct {
	uint32_t flags;                /**< Flags regarding volume state */
	const vfs_volume_funcs *funcs; /**< Volume-managing functions */
} vfs_volume;

// Indicates an opened file
#define VFS_FILE_OPEN  (1 << 0)
// Indicates read permission on file
#define VFS_FILE_READ  (1 << 1)
// Indicates write permission on file
#define VFS_FILE_WRITE (1 << 2)
// Set if EOF has been reached
#define VFS_EOF        (1 << 3)

/**
 * Contains data to manage open files.
 */
typedef struct {
	uint32_t flags;     /**< File attribute flags */
	uint32_t vol;       /**< Index of volume file is stored on */
	uint32_t pid;       /**< PID of process handling this file */
	vfs_file_info info; /**< File info used by file IO functions */
} vfs_file;

/**
 * Initializes the filesystem abstraction layer.
 */
void vfs_init(void);

int vfs_mount(const vfs_volume_funcs *funcs, uint32_t flags);
int vfs_open(const char *path, uint32_t flags);
int vfs_close(int fd);
uint32_t vfs_read(int fd, uint32_t count, uint8_t *buffer);
uint32_t vfs_write(int fd, uint32_t count, const uint8_t *buffer);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int vfs_seek(int fd, int32_t offset, int whence);
int32_t vfs_tell(int fd);

#endif // VFS_H_

