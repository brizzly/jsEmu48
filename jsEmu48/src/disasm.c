/*
 *     /
 *    /__  ___  ___  ____
 *   /  / /  / /__/ / / / /  /
 *  /  / /__/ /__  /   / /__/
 *      /
 *     /    version 0.9.0
 *
 * Copyright 2002 Daniel Nilsson
 *
 * This file is part of hpemu.
 *
 * Hpemu is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hpemu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hpemu; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include "types.h"
#include "opcodes.h"
#include "disasm.h"

static const char hex_tab[16] = "0123456789ABCDEF";

static const char r_tab[11][16] = {
    "ABCDABCDBCACBCAC",	// p
    "BCAC1111ABCDABCD",	// q
    "ABCDBCACABCDBCAC",	// r
    "BCACABCDBCACABCD",	// s
    "ABCDABCDBCACABCD",	// t
    "ABCDABCDABCDABCD",	// u (different than in opcodes.c)
    "BCACBCACBCACBCAC",	// v (different than in opcodes.c)
    "BCACABCDABCD1111",	// w
    "ABCDABCDBCACABAC",	// x
    "0000BCACABCDBCCD",	// y
    "BCACBCAC00000000",	// z
};

static const char *f_tab[8] = { "P", "WP", "XS", "X", "S", "M", "B", "W" };

#define HEX(x)		(hex_tab[x])

#define ADD_OFFSET(x,s)	do { if (*(s) == '+') {		\
			   (s)++;			\
			   (x) += *(s)++ - '0';		\
			} } while (0)

static __inline int nib_to_signed(byte *nib, int len)
{
    int x;

    len--;
    x = nib[len];
    if (x & 8) x -= 16;
    while (len--) {
	x <<= 4;
	x |= nib[len];
    }
    return x;
}

static void expand(char *dest, char *src, byte *ptr)
{
    int i, n;
    int x;

    while (*src) {
	if (*src != '%') {
	    *dest++ = *src++;
	} else {
	    src++;
	    switch (*src++) {

	    case 'I':	// One digit integer
		i = *src++ - '0';
		x = ptr[i];
		ADD_OFFSET(x, src);
		if (x >= 10) {
		    *dest++ = '0' + x / 10;
		    *dest++ = '0' + x % 10;
		} else {
		    *dest++ = '0' + x;
		}
		break;

	    case 'X':	// Hexadecimal number
		i = *src++ - '0';
		n = *src++ - '0';
		while (n--) {
		    *dest++ = HEX(ptr[i+n]);
		}
		break;

	    case 'N':	// Variable length hexadecimal number
		i = *src++ - '0';
		n = ptr[i] + 1;
		while (n) {
		    *dest++ = HEX(ptr[i+n]);
		    n--;
		}
		break;

	    case 'R':	// Relative address
		i = *src++ - '0';
		n = *src++ - '0';
		x = nib_to_signed(ptr + i, n);
		ADD_OFFSET(x, src);
		if (x < 0) {
		    *dest++ = '-';
		    x = -x;
		} else {
		    *dest++ = '+';
		}
		while (n--) {
		    *dest++ = HEX((x >> (n*4)) & 0xF);
		}
		break;

	    case 'T':	// Relative address for tests
		i = *src++ - '0';
		n = *src++ - '0';
		x = nib_to_signed(ptr + i, n);
		if (x == 0) {
		    ADD_OFFSET (x, src);    // skip over the optional "+i"
		    break;
		}
		ADD_OFFSET (x, src);
		if (x < 0) {
		    *dest++ = '-';
		    x = -x;
		} else {
		    *dest++ = '+';
		}
		while (n--) {
		    *dest++ = HEX((x >> (n*4)) & 0xF);
		}
		break;

	    case 'G':	// "GO" or "RTN" for tests
		i = *src++ - '0';
		n = *src++ - '0';
		x = nib_to_signed(ptr + i, n);
		if (x == 0) {
		    *dest++ = 'R';
		    *dest++ = 'T';
		    *dest++ = 'N';
		} else {
		    *dest++ = 'G';
		    *dest++ = 'O';
		}
		break;

	    case 'F':	// Field name
		i = *src++ - '0';
		if (*src == 'A') {
		    src++;
		    if (ptr[i] == 0xF) {
			*dest++ = 'A';
			break;
		    }
		}
		*dest++ = f_tab[ptr[i]&7][0];
		if (f_tab[ptr[i]&7][1])
		    *dest++ = f_tab[ptr[i]&7][1];
		break;

	    case 'B':	// Field A or B
		i = *src++ -'0';
		*dest++ = (ptr[i]&8) ? 'B' : 'A';
		break;

	    case 'p':	// Subtraction source 1
	    case 'q':	// Subtraction source 2
	    case 'r':	// Logic destination & source 1
	    case 's':	// Logic source 2
	    case 't':	// Arithmethic destination & addition source 1
	    case 'u':	// Compare source 1
	    case 'v':	// Compare source 2 for <, >, <= and >=
	    case 'w':	// Addition source 2
	    case 'x':	// Assignment destination
	    case 'y':	// Assignment source
	    case 'z':	// Compare source 2 for = and #
		i = *src++ - '0';
		*dest++ = r_tab[src[-2]-'p'][ptr[i]];
		break;

	    case 'a':	// "A" or "C"
		i = *src++ - '0';
		*dest++ = (ptr[i]&8) ? 'C' : 'A';
		break;

	    case 'i':	// R-register number
		i = *src++ - '0';
		*dest++ = '0' + ((ptr[i]&7) > 4 ? ptr[i]&3 : ptr[i]&7);
		break;

	    case '#':	// "#" or "=" for comparisons
		i = *src++ - '0';
		*dest++ = (ptr[i]&4) ? '#' : '=';
		break;

	    case '>':	// ">", "<", ">=" or "<=" for comparisons
		i = *src++ - '0';
		*dest++ = (ptr[i] & 4) ? '<' : '>';
		if (ptr[i] & 8) *dest++ = '=';
		break;

	    case '=':	// Conditional "=" for assignments
		i = *src++ - '0';
		if (ptr[i] < 0xC) *dest++ = '=';
		break;

	    case 'E':	// Conditionsl "EX" for assignments
		i = *src++ - '0';
		if (ptr[i] >= 0xC) {
		    *dest++ = 'E';
		    *dest++ = 'X';
		}
		break;

	    case '+':	// "+" or "-" for add / dec
		i = *src++ - '0';
		*dest++ = (ptr[i] < 0xC) ? '+' : '-';
		break;

	    case '-':	// "-" or "+" for sub / inc
		i = *src++ - '0';
		*dest++ = ((ptr[i]&0xC) != 0x4) ? '-' : '+';
		break;

	    case '~':	// "+" or "-" for +con and -con
		i = *src++ - '0';
		*dest++ = (ptr[i]&8) ? '-' : '+';
		break;

	    case '&':	// "&" or "!" for and / or
		i = *src++ - '0';
		*dest++ = (ptr[i]&8) ? '!' : '&';
		break;

	    case 'D':	// DATi=r or r=DATi
		i = *src++ - '0';
		if (!(ptr[i]&2)) {
		    *dest++ = 'D';
		    *dest++ = 'A';
		    *dest++ = 'T';
		    *dest++ = '0' + (ptr[i]&1);
		    *dest++ = '=';
		    *dest++ = 'A' + ((ptr[i]&4)>>1);
		} else {
		    *dest++ = 'A' + ((ptr[i]&4)>>1);
		    *dest++ = '=';
		    *dest++ = 'D';
		    *dest++ = 'A';
		    *dest++ = 'T';
		    *dest++ = '0' + (ptr[i]&1);
		}
		break;

	    case '%':	// Litteral %
		*(dest++) = '%';
		break;
	    }
	}
    }
    *dest = '\0';
}

char *disassemble(byte *ptr)
{
    static char buffer[64];

    Opcode *op = opcodes;
    int i = 0;

    while (op[ptr[i]].next) {
	op = op[ptr[i]].next;
	i++;
    }
    if (op[ptr[i]].dissasm) {
	expand(buffer, op[ptr[i]].dissasm, ptr);
    } else {
	sprintf (buffer, "Unknown (%s)", nib_to_hex(ptr, i+1));
    }
    return buffer;
}

char *nib_to_hex(byte *nib, int n)
{
    static char buffer[17];

    buffer[n] = '\0';
    while (n--) {
	buffer[n] = HEX(nib[n]);
    }
    return buffer;
}

char *nib_to_hex_rev(byte *nib, int n)
{
    static char buffer[17];

    buffer[n] = '\0';
    while (n--) {
	buffer[n] = HEX(*nib++);
    }
    return buffer;
}

