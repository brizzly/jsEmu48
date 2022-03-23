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

#include <stdlib.h>
#include "types.h"
#include "bus.h"
#include "opinline.h"
#include "rpl.h"

#define TEMPOB	    0x806E9
#define TEMPTOP	    0x806EE
#define RSKTOP	    0x806F3
#define DSKTOP	    0x806F8
#define AVMEM	    0x807ED
#define INTRPPTR    0x8072F

int rpl_object_size(byte *obj)
{
    int size;
    int prologue;
    int n;

    prologue = nib_to_unsigned(obj, 5);

    switch (prologue) {

    case 0x0312B: size = 0;  break; // SEMI
    case 0x029BF: size = 7;  break; // Character
    case 0x02911: size = 10; break; // System binary integer
    case 0x02E92: size = 11; break; // XLIB name
    case 0x02BAA: size = 15; break; // Extended pointer
    case 0x02933: size = 21; break; // Real number
    case 0x02955: size = 26; break; // Long real
    case 0x02977: size = 37; break; // Complex number
    case 0x0299D: size = 47; break; // Long complex

    case 0x02A2C:   // String
    case 0x02A4E:   // User binary integer
    case 0x02B1E:   // Graphic object
    case 0x02DCC:   // Code object
    case 0x02B88:   // Library data
    case 0x029E8:   // Array
    case 0x02B40:   // Library
    case 0x02B62:   // Backup
    case 0x02A0A:   // Linked array
	size = 5 + nib_to_unsigned(obj+5, 5);
	break;

    case 0x02E48:   // Global name
    case 0x02E6D:   // Local name
    case 0x02AFC:   // Tagged object
    case 0x02ADA:   // Unit object
	size = 7 + 2 * nib_to_unsigned(obj+5, 2);
	if (prologue == 0x02AFC) {  // Tagged object
	    size += rpl_object_size(obj + size);
	}
	break;

    case 0x02A74:   // List
    case 0x02D9D:   // RPL program
    case 0x02AB8:   // Algebraic expression
	size = 5;
	n = 5;
	do {
	    size += n;
	    obj += n;
	    n = rpl_object_size(obj);
	} while (n > 0);
	break;

    case 0x02A96:   // Directory
	n = nib_to_unsigned(obj+8, 5);
	if (n == 0) {
	    size = 13;
	} else {
	    size = 8 + n;
	    size += 4 + 2 * nib_to_unsigned(obj + size, 2);
	    size += rpl_object_size(obj + size);
	}
	break;

    default:
	size = 5;
    }
    return size;
}

static address read_address(address adr)
{
    byte buf[5];
    word ocrc;

    ocrc = crc;
    bus_read(buf, adr, 5);
    crc = ocrc;

    return nib_to_unsigned(buf, 5);
}

static void write_address(address adr, address val)
{
    byte buf[5];

    unsigned_to_nib(buf, val, 5);
    bus_write(buf, adr, 5);
}

static int moveup(address src, address dst, address cnt)
{
    byte *buf = malloc(cnt * sizeof(byte));
    word ocrc;

    if (!buf)
	return -1;

    ocrc = crc;
    bus_read(buf, src, cnt);
    bus_write(buf, dst, cnt);
    crc = ocrc;

    free(buf);
    return 0;
}

address rpl_make_temp(address size)
{
    address temptop, rsktop, dsktop;

    size += 6;

    temptop = read_address(TEMPTOP);
    rsktop = read_address(RSKTOP);
    dsktop = read_address(DSKTOP);

    if (rsktop + size > dsktop)
	return 0;

    if (moveup(temptop, temptop + size, rsktop - temptop))
	return 0;

    write_address(TEMPTOP, temptop + size);
    write_address(RSKTOP, rsktop + size);
    write_address(AVMEM, (dsktop - rsktop - size) / 5);
    write_address(temptop + size - 5, size);

    return temptop + 1;
}

void rpl_push(address adr)
{
    address dsktop, avmem;

    avmem = read_address(AVMEM);
    if (!avmem)
	return;
    write_address(AVMEM, avmem-1);

    dsktop = read_address(DSKTOP);
    dsktop -= 5;
    write_address(dsktop, adr);
    write_address(DSKTOP, dsktop);
}

int rpl_push_object(byte *obj, address size)
{
    address adr;

    adr = rpl_make_temp(size);
    if (!adr)
	return -1;

    bus_write(obj, adr, size);
    rpl_push(adr);
    return 0;
}
