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

#ifndef __CPU_H
#define __CPU_H

#include "types.h"

typedef struct {
    byte reg[4][16];
    byte reg_r[5][16];
    byte st[4];
    byte in[4];
    byte out[3];
    byte hst;
    byte p;
    address d[2];
    address pc;
    address rstk[8];
    int rstk_ptr;
    boolean carry	:1;
    boolean dec		:1;
    boolean shutdown	:1;
    boolean inte	:1;
    boolean keyscan	:1;
    boolean keyintp	:1;
    dword cycles;
    dword inst_cnt;
} Cpu;

#define HST_XM	1
#define HST_SB	2
#define HST_SR	4
#define HST_MP	8

enum RegisterNames { A, B, C, D };

extern Cpu cpu;

void cpu_interrupt(void);
void execute_instruction(void);

#endif
