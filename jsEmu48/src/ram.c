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
#include <string.h>
#include "types.h"
#include "bus.h"
#include "ram.h"

static address ram_size = 256*1024;	// in nibbles, not bytes!

void ram_init(void)
{
    byte *buf;

    buf = malloc(ram_size);
    if (!buf) {
	exit(0x20);
    }
    memset(buf, 0, ram_size);
    bus_info.ram_data = buf;
    bus_info.ram_mask = ram_size-1;
}

void ram_exit(void)
{
    free(bus_info.ram_data);
    bus_info.ram_data = NULL;
    bus_info.ram_mask = 0x00000;
}
