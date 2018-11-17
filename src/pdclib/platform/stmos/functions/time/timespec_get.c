/* timespec_get( struct timespec *, int )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>

#ifndef REGTEST

#include <syscalls.h>

int timespec_get( struct timespec * ts, int base )
{
    if ( base == TIME_UTC )
    {
	unsigned int t = ticks();
        ts->tv_sec = t / 1000;
	ts->tv_nsec = t * 1000000;
        return base;
    }

    /* Not supporting any other time base than TIME_UTC for now. */
    return 0;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    TESTCASE( NO_TESTDRIVER );
    return TEST_RESULTS;
}

#endif
