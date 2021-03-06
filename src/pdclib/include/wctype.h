/* Wide character classification and mapping utilities <wctype.h>

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_WCTYPE_H
#define _PDCLIB_WCTYPE_H _PDCLIB_WCTYPE_H

#include "pdclib/_PDCLIB_int.h"

typedef _PDCLIB_wint_t wint_t;

// wctrans_t
// wctype_t

#ifndef _PDCLIB_WEOF_DEFINED
#define _PDCLIB_WEOF_DEFINED _PDCLIB_WEOF_DEFINED
#define WEOF (wint_t)-1
#endif

/* Wide character classification functions */

/* Returns iswalpha( wc ) || iswdigit( wc ) */
int iswalnum( wint_t wc );

/* Returns true for wide characters for which either isupper( wc ) or
   islower( wc ) is true, as well as a set of locale-specific wide
   characters which are neither control characters, digits, punctuation,
   or whitespace.
*/
int iswalpha( wint_t wc );

/* Returns true if the character iswspace() and used for separating words
   within a line of text. In the "C" locale, only L' ' and L'\t' are
   considered blanks.
*/
int iswblank( wint_t wc );

/* Returns true if the wide character is a control character. */
int iswcntrl( wint_t wc );

/* Returns true if the wide character is a decimal digit. Locale-
   independent. */
int iswdigit( wint_t wc );

/* Returns iswprint( wc ) && ! iswspace( wc ).
   NOTE: This definition differs from that of isgraph() in <ctype.h>,
         which considers only ' ', not all isspace() characters.
*/
int iswgraph( wint_t wc );

/* Returns true for lowerspace wide characters, as well as a set of
   locale-specific wide characters which are neither control charcters,
   digits, punctuation, or whitespace.
*/
int iswlower( wint_t wc );

/* Returns true for every printing wide character. */
int iswprint( wint_t wc );

/* Returns true for a locale-specific set of punctuation characters that
   are neither whitespace nor alphanumeric.
*/
int iswpunct( wint_t wc );

/* Returns true for a locale-specific set of whitespace characters that
   are neither alphanumeric, graphic, or punctuation.
*/
int iswspace( wint_t wc );

/* Returns true for upperspace wide characters, as well as a set of
   locale-specific wide characters which are neither control charcters,
   digits, punctuation, or whitespace.
*/
int iswupper( wint_t wc );

/* Returns true if the wide character is a hexadecimal digit. Locale-
   independent. */
int iswxdigit( wint_t wc );

/* Extensible wide character classification functions */

/* Returns true if the wide character wc has the property described by
   desc (which was retrieved by a previous call to wctype() without
   changing the LC_CTYPE locale setting between the two calls).
*/
int iswctype( wint_t wc, wctype_t desc );

/* Returns a description object for a named character property, to be
   used as parameter to the iswctype() function. Supported property
   names are:
   "alnum" -- alphanumeric, as per iswalnum()
   "alpha" -- alphabetic, as per iswalpha()
   "blank" -- blank, as per iswblank()
   "cntrl" -- control, as per iswcntrl()
   "digit" -- decimal digit, as per iswdigit()
   "graph" -- graphic, as per iswgraph()
   "lower" -- lowercase, as per iswlower()
   "print" -- printing, as per iswprint()
   "punct" -- punctuation, as per iswprint()
   "space" -- whitespace, as per iswspace()
   "upper" -- uppercase, as per iswupper()
   "xdigit" -- hexadecimal digit, as per iswxdigit()
   For unsupported properties, the function returns zero.
*/
wctype_t wctype( const char * property );

/* Wide character case mapping utilities */

/* Converts an uppercase letter to a corresponding lowercase letter. Input for
   which no corresponding lowercase letter exists remains unchanged.
*/
wint_t towlower( wint_t wc );

/* Converts a lowercase letter to a corresponding uppercase letter. Input for
   which no corresponding uppercase letter exists remains unchanged.
*/
wint_t towupper( wint_t wc );

/* Extensible wide character case mapping utilities */

/* Converts the wide character wc according to the transition described
   by desc (which was retrieved by a previous call to wctrans() without
   changing the LC_CTYPE locale setting between the two calls).
*/
wint_t towctrans( wint_t wc, wctrans_t desc );

/* Returns a description object for a named character transformation, to
   be used as parameter to the towctrans() function. Supported transformation
   properties are:
   "tolower" -- lowercase mapping, as per towlower()
   "toupper" -- uppercase mapping, as per towupper()
   For unsupported properties, the function returns zero.
*/
wctrans_t wctrans( const char * property );

#endif
