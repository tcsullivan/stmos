/* Errors <errno.h>

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_ERRNO_H
#define _PDCLIB_ERRNO_H _PDCLIB_ERRNO_H

#include "pdclib/_PDCLIB_int.h"

#define errno (*_PDCLIB_errno_func())

#define ERANGE _PDCLIB_ERANGE
#define EDOM _PDCLIB_EDOM

#endif
