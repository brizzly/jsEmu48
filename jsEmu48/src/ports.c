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
#include "ports.h"

static byte current_bank;
static byte *port2;
static address port2mask;

void ports_init(void)
{
    // ce1 = bank switcher
    bus_info.ce1_data = NULL;
    bus_info.ce1_mask = 0x0007F;
    bus_info.ce1_r_o = TRUE;
    bus_info.ce1_bs = TRUE;

#if 1
    // ce2 = port1 (unplugged)
    bus_info.ce2_data = NULL;
    bus_info.ce2_mask = 0x00000;
    bus_info.ce2_r_o = TRUE;
#else
#define PORT1_SIZE  (64*1024)	// Nibbles
    // ce2 = port1 (plugged)
    bus_info.ce2_data = malloc(PORT1_SIZE);
    bus_info.ce2_mask = PORT1_SIZE-1;
    bus_info.ce2_r_o = FALSE;
#endif

#if 1
    // nce3 = port2 (unplugged)
    port2 = NULL;
    port2mask = 0x00000;
    bus_info.nce3_data = port2;
    bus_info.nce3_mask = port2mask & 0x3FFFF;
    bus_info.nce3_r_o = TRUE;
#else
#define PORT2_SIZE  (512*1024)   // Nibbles
    // nce3 = port2 (plugged)
    port2 = malloc(PORT2_SIZE);
    port2mask = PORT2_SIZE-1;
    bus_info.nce3_data = port2;
    bus_info.nce3_mask = port2mask & 0x3FFFF;
    bus_info.nce3_r_o = FALSE;
#endif

    bus_info.ben = FALSE;
    current_bank = 0;
}

void ports_exit(void)
{
}

void ports_switch_bank(address adr)
{
    boolean need_remap = FALSE;

    if (current_bank != (((byte) adr >> 1) & 0x1F)) {
	current_bank = ((byte) adr >> 1) & 0x1F;
	if (port2) {
	    bus_info.nce3_data = port2 + ((current_bank << 18) & port2mask);
	    if (bus_info.nce3_cfg) {
		need_remap = TRUE;
	    }
	}
    }
    if (!bus_info.ben != !(adr & 0x40)) {
	bus_info.ben = (adr & 0x40) ? TRUE : FALSE;
	if (bus_info.nce3_cfg) {
	    need_remap = TRUE;
	}
    }
    if (need_remap) {
	bus_remap();
    }
}

byte ports_card_detect(void)
{
    byte x = 0;
    if (bus_info.nce3_data) x |= 0x1;
    if (bus_info.ce2_data) x |= 0x2;
    if (!bus_info.nce3_r_o) x |= 0x4;
    if (!bus_info.ce2_r_o) x |= 0x8;
    return x;
}
