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

#ifndef __BUS_H
#define __BUS_H

#include "types.h"


#define SEG_OF(adr)	((adr)>>12)
#define OFFSET_OF(adr)	((adr)&0xFFF)
#define CAN_READ(adr)	(read_map[SEG_OF(adr)] != NULL)
#define CAN_WRITE(adr)	(write_map[SEG_OF(adr)] != NULL)
#define MAP_READ(adr)	(read_map[SEG_OF(adr)]+OFFSET_OF(adr))
#define MAP_WRITE(adr)	(write_map[SEG_OF(adr)]+OFFSET_OF(adr))

#define MIN(a,b)	((a)<(b)?(a):(b))



typedef struct {
    address hdw_base;
    address ram_size;
    address ram_base;
    address ce1_size;
    address ce1_base;
    address ce2_size;
    address ce2_base;
    address nce3_size;
    address nce3_base;
    boolean hdw_cfg	:1;
    boolean ram_sz_cfg	:1;
    boolean ram_cfg	:1;
    boolean ce1_sz_cfg	:1;
    boolean ce1_cfg	:1;
    boolean ce2_sz_cfg	:1;
    boolean ce2_cfg	:1;
    boolean nce3_sz_cfg	:1;
    boolean nce3_cfg	:1;
    boolean ce1_r_o	:1;
    boolean ce2_r_o	:1;
    boolean nce3_r_o	:1;
    boolean ce1_bs	:1;
    boolean da19	:1;
    boolean ben		:1;
    byte *rom_data;
    byte *ram_data;
    byte *ce1_data;
    byte *ce2_data;
    byte *nce3_data;
    address rom_mask;
    address ram_mask;
    address ce1_mask;
    address ce2_mask;
    address nce3_mask;
    dword map_cnt;
} BusInfo;

extern BusInfo bus_info;

extern word crc;

// FAST_PEEK_MAX must not be greater than the size of the hdw registers (64)
#define FAST_PEEK_MAX	64

void bus_init(void);
void bus_exit(void);

void bus_read(byte *buf, address adr, address len);
void bus_write(byte *buf, address adr, address len);
byte *bus_fast_peek(byte *buf, address adr, address *len);

void bus_remap(void);
void bus_configure(address adr);
void bus_unconfigure(address adr);
void bus_reset(void);
address bus_get_id();

#endif
