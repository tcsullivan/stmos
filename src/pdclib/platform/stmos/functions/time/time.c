/* time( time_t * )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>

#ifndef REGTEST

#include <syscalls.h>

/* See comments in time.h on the semantics of time_t. */

time_t time( time_t * timer )
{
    if ( timer != NULL )
        *timer = ticks();

    return ticks();
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
