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

#include "types.h"
#include "cpu.h"
#include "keyboard.h"

boolean kbd_on;

static byte kbd_row[9];

static void update_in(void)
{
    byte in = 0;

    // TODO: Emulate real HP48 keyboard circuit

    if (cpu.out[0]&1) in |= kbd_row[0];
    if (cpu.out[0]&2) in |= kbd_row[1];
    if (cpu.out[0]&4) in |= kbd_row[2];
    if (cpu.out[0]&8) in |= kbd_row[3];
    if (cpu.out[1]&1) in |= kbd_row[4];
    if (cpu.out[1]&2) in |= kbd_row[5];
    if (cpu.out[1]&4) in |= kbd_row[6];
    if (cpu.out[1]&8) in |= kbd_row[7];
    if (cpu.out[2]&1) in |= kbd_row[8];

    cpu.in[0] = in & 0xF;
    cpu.in[1] = (in >> 4) & 0xF;
    cpu.in[2] = 0;
    cpu.in[3] = kbd_on ? 8 : 0;
}

void kbd_out_changed(void)
{
    update_in();
}

void kbd_key_pressed(int row, int col)
{
    boolean no_key = !cpu.in[0] && !cpu.in[1] && !cpu.in[3];
    kbd_row[row] |= 1 << col;
    update_in();
    if (cpu.shutdown && no_key && (cpu.in[0] || cpu.in[1] || cpu.in[3])) {
	cpu.shutdown = FALSE;
    }
    if (cpu.keyscan && no_key && (cpu.in[0] || cpu.in[1] || cpu.in[3])) {
	if (cpu.inte) {
	    cpu.keyintp = FALSE;
	    cpu_interrupt();
	} else {
	    cpu.keyintp = TRUE;
	}
    } else if (!cpu.in[0] && !cpu.in[1] && !cpu.in[3]) {
	cpu.keyintp = FALSE;
    }
}

void kbd_key_released(int row, int col)
{
    kbd_row[row] &= ~(1 << col);
    update_in();
    if (!cpu.in[0] && !cpu.in[1] && !cpu.in[3]) {
	cpu.keyintp = FALSE;
    }
}

void kbd_on_pressed(void)
{
    boolean no_key = !cpu.in[3];
    kbd_on = TRUE;
    cpu.in[3] |= 8;
    if (cpu.shutdown && no_key) {
	cpu.shutdown = FALSE;
    }
    if (cpu.inte && no_key) {
	cpu_interrupt();
    }
}

void kbd_on_released(void)
{
    kbd_on = FALSE;
    cpu.in[3] &= ~8;
}
