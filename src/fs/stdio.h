/**
 * @file stdio.h
 * Filesystem module for handling stdio (stdout, stdin, stderr)
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

#ifndef INTIRD_H_
#define INITRD_H_

#include <kernel/vfs.h>

/**
 * Structure of function calls for stdio filesystem operations.
 */
extern const vfs_volume_funcs stdio_funcs;

#endif // INITRD_H_
