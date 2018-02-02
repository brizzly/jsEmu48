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

#include <string.h>
#include "types.h"
#include "rom.h"
#include "ram.h"
#include "ports.h"
#include "hdw.h"
#include "bus.h"

BusInfo bus_info = {
//  hdw	    ram sz  ram	    ce1 sz  ce1	    ce2 sz  ce2	    nce3 sz nce3
    0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,	// base or size
    FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,  FALSE,	// configured
			    FALSE,	    FALSE,	    FALSE,		// read only
//  ce1_bs  da19    ben
    FALSE,  FALSE,  FALSE,
//  rom	    ram	    ce1	    ce2	    nce3
    NULL,   NULL,   NULL,   NULL,   NULL,	// data
    0x00000,0x00000,0x00000,0x00000,0x00000	// mask
};

static byte *read_map[256];
static byte *write_map[256];
static address hdw_seg;

word crc;

void bus_init(void)
{
    rom_init();
    hdw_init();
    ram_init();
    ports_init();
    bus_reset();
}

void bus_exit(void)
{
    rom_exit();
    hdw_exit();
    ram_exit();
    ports_exit();
}

static __inline void update_crc(byte nibble)
{
    crc = (crc >> 4) ^ (((crc ^ nibble) & 0xF) * 0x1081);
}

void bus_read(byte *buf, address adr, address len)
{
    int n, i;

    while (TRUE) {
	if (hdw_seg == SEG_OF(adr) && ((bus_info.hdw_base ^ adr) & 0xFFFC0) == 0) {
	    n = MIN(len, 0x40 - (adr & 0x3F));
	    for (i = 0; i < n; i++) {
		buf[i] = hdw_read_nibble((adr & 0x3F) + i);
	    }
	    if (((adr & 0x3F) + n) == 0x40) {
		update_crc(buf[n-1]);
	    }
	} else {
	    if (hdw_seg == SEG_OF(adr) && (bus_info.hdw_base & 0xFFFC0) - adr > 0) {
		n = MIN(len, (bus_info.hdw_base & 0xFFFC0) - adr);
	    } else {
		n = MIN(len, 0x1000 - OFFSET_OF (adr));
	    }
	    if (CAN_READ(adr)) {
		memcpy(buf, MAP_READ(adr), n);
	    } else {
		for (i = 0; i < n; i++) {
		    buf[i] = ((i+adr)&1) ? 0xE : 0xD;
		}
		if (bus_info.ce1_bs && bus_info.ce1_cfg &&
		    ((bus_info.ce1_base ^ adr) & bus_info.ce1_size)) {
		    ports_switch_bank(OFFSET_OF(adr+n));
		}
	    }
	    for (i = 0; i < n; i++) {
		update_crc(buf[i]);
	    }
	}
	len -= n;
	if (!len) {
	    break;
	}
	buf += n;
	adr += n;
	adr &= 0xFFFFF;
    }
}

void bus_write(byte *buf, address adr, address len)
{
    int n, i;

    while (TRUE) {
	if (hdw_seg == SEG_OF(adr) && ((bus_info.hdw_base ^ adr) & 0xFFFC0) == 0) {
	    n = MIN(len, 0x40 - (adr & 0x3F));
	    for (i = 0; i < n; i++) {
		hdw_write_nibble(buf[i], (adr & 0x3F) + i);
	    }
	} else {
	    if (hdw_seg == SEG_OF(adr) && (bus_info.hdw_base & 0xFFFC0) - adr > 0) {
		n = MIN(len, (bus_info.hdw_base & 0xFFFC0) - adr);
	    } else {
		n = MIN(len, 0x1000 - OFFSET_OF (adr));
	    }
	    if (CAN_WRITE(adr)) {
		memcpy (MAP_WRITE(adr), buf, n);
	    } else if (bus_info.ce1_bs) {
		if (bus_info.ce1_cfg && ((bus_info.ce1_base ^ adr) & bus_info.ce1_size)) {
		    if (!bus_info.nce3_r_o) {
			ports_switch_bank(OFFSET_OF(adr+n-1));
		    } else if ((adr+n) & 1) {
			ports_switch_bank(OFFSET_OF(adr+n-1));
		    } else if (adr & 0x1) {
			ports_switch_bank(OFFSET_OF(adr));
		    }
		}
	    }
	}
	len -= n;
	if (!len) {
	    break;
	}
	buf += n;
	adr += n;
	adr &= 0xFFFFF;
    }
}

static void bus_peek(byte *buf, address adr, address len)
{
    int n, i;

    while (TRUE) {
	if (hdw_seg == SEG_OF(adr) && ((bus_info.hdw_base ^ adr) & 0xFFFC0) == 0) {
	    n = MIN(len, 0x40 - (adr & 0x3F));
	    for (i = 0; i < n; i++) {
		buf[i] = hdw_read_nibble((adr & 0x3F) + i);
	    }
	} else {
	    if (hdw_seg == SEG_OF(adr) && (bus_info.hdw_base & 0xFFFC0) - adr > 0) {
		n = MIN(len, (bus_info.hdw_base & 0xFFFC0) - adr);
	    } else {
		n = MIN(len, 0x1000 - OFFSET_OF (adr));
	    }
	    if (CAN_READ(adr)) {
		memcpy(buf, MAP_READ(adr), n);
	    } else {
		for (i = 0; i < n; i++) {
		    buf[i] = ((i+adr)&1) ? 0xE : 0xD;
		}
	    }
	}
	len -= n;
	if (!len) {
	    break;
	}
	buf += n;
	adr += n;
	adr &= 0xFFFFF;
    }
}

/* Call only when you know that hdw is not in the range of nibbles read */
static void bus_peek_no_hdw(byte *buf, address adr, address len)
{
    int n, i;

    while (TRUE) {
	n = MIN(len, 0x1000 - OFFSET_OF(adr));
	if (CAN_READ(adr)) {
	    memcpy(buf, MAP_READ(adr), n);
	} else {
	    for (i = 0; i < n; i++) {
		buf[i] = ((i+adr)&1) ? 0xE : 0xD;
	    }
	}
	len -= n;
	if (!len) {
	    break;
	}
	buf += n;
	adr += n;
	adr &= 0xFFFFF;
    }
}


/* Returns a pointer for fast read access to memory (don't write).
 * If a direct pointer can't be returnd it will use the caller supplied buffer instead.
 * If the buffer is not used the pointer returned is valid as long as bus_info.map_cnt
 * has the same value (until a call to bus_remap).
 * If the buffer is used the pointer returned (== buf) is valid only until the memory
 * is written to.
 * Set len to the number of nibbles you want to access. It is changed to the actual number of
 * nibbles that can safetly be read through the pointer (can be more or less then the original).
 */
byte *bus_fast_peek(byte *buf, address adr, int *len)
{
    static byte tmp_buf[FAST_PEEK_MAX];
    static int tmp_len;
    address adr2;

    if (!buf) {
	buf = tmp_buf;
    }
    if (!len) {
	tmp_len = FAST_PEEK_MAX;
	len = &tmp_len;
    }
    if (*len > FAST_PEEK_MAX) {
	*len = FAST_PEEK_MAX;
    }

    adr2 = adr + *len - 1;

    if ((SEG_OF(adr) == hdw_seg || SEG_OF(adr2) == hdw_seg) &&
	(((bus_info.hdw_base ^ adr) & 0xFFFC0) == 0 ||
	 ((bus_info.hdw_base ^ adr2) & 0xFFFC0) == 0)) {
	bus_peek(buf, adr, *len);
	return buf;
    } else if (!CAN_READ(adr)) {
	bus_peek_no_hdw(buf, adr, *len);
	return buf;
    } else if (SEG_OF(adr) == SEG_OF(adr2)) {
	if (hdw_seg == SEG_OF(adr) && (bus_info.hdw_base & 0xFFFC0) - adr > 0) {
	    *len = (bus_info.hdw_base & 0xFFFC0) - adr;
	} else {
	    *len = 0x1000 - OFFSET_OF (adr);
	}
	return MAP_READ(adr);
    } else if (CAN_READ(adr2) && MAP_READ(adr) + *len-1 == MAP_READ(adr2)) {
	if (hdw_seg == SEG_OF(adr2)) {
	    *len = (bus_info.hdw_base & 0xFFFC0) - adr;
	} else {
	    *len = 0x2000 - OFFSET_OF(adr);
	}
	return MAP_READ(adr);
    } else {
	bus_peek_no_hdw(buf, adr, *len);
	return buf;
    }
}

void bus_remap(void)
{
    int adr;

    for (adr = 0; adr < 0x100000; adr += 0x01000) {
	if (bus_info.ram_cfg && ((bus_info.ram_base ^ adr) & bus_info.ram_size) == 0) {
	    read_map[SEG_OF(adr)] = bus_info.ram_data + (adr & bus_info.ram_mask);
	    write_map[SEG_OF(adr)] = bus_info.ram_data + (adr & bus_info.ram_mask);
	} else if (bus_info.ce2_cfg && ((bus_info.ce2_base ^ adr) & bus_info.ce2_size) == 0) {
	    if (bus_info.ce2_data) {
		read_map[SEG_OF(adr)] = bus_info.ce2_data + (adr & bus_info.ce2_mask);
		if (!bus_info.ce2_r_o) {
		    write_map[SEG_OF(adr)] = bus_info.ce2_data + (adr & bus_info.ce2_mask);
		} else {
		    write_map[SEG_OF(adr)] = NULL;
		}
	    } else {
		read_map[SEG_OF(adr)] = NULL;
		write_map[SEG_OF(adr)] = NULL;
	    }
	} else if (bus_info.ce1_cfg && ((bus_info.ce1_base ^ adr) & bus_info.ce1_size) == 0) {
	    if (bus_info.ce1_data) {
		read_map[SEG_OF(adr)] = bus_info.ce1_data + (adr & bus_info.ce1_mask);
		if (!bus_info.ce1_r_o) {
		    write_map[SEG_OF(adr)] = bus_info.ce1_data + (adr & bus_info.ce1_mask);
		} else {
		    write_map[SEG_OF(adr)] = NULL;
		}
	    } else {
		read_map[SEG_OF(adr)] = NULL;
		write_map[SEG_OF(adr)] = NULL;
	    }
	} else if (bus_info.nce3_cfg && ((bus_info.nce3_base ^ adr) & bus_info.nce3_size) == 0) {
	    if (bus_info.nce3_data && bus_info.ben && !bus_info.da19) {
		read_map[SEG_OF(adr)] = bus_info.nce3_data + (adr & bus_info.nce3_mask);
		if (!bus_info.nce3_r_o) {
		    write_map[SEG_OF(adr)] = bus_info.nce3_data + (adr & bus_info.nce3_mask);
		} else {
		    write_map[SEG_OF(adr)] = NULL;
		}
	    } else {
		read_map[SEG_OF(adr)] = NULL;
		write_map[SEG_OF(adr)] = NULL;
	    }
	} else {
	    read_map[SEG_OF(adr)] = bus_info.rom_data + (adr & bus_info.rom_mask & (bus_info.da19 ? 0xFFFFF : 0x7FFFF));
	    write_map[SEG_OF(adr)] = NULL;
	}
    }
    bus_info.map_cnt++;	// Flag that pointers returned by bus_fast_peek may be invalid
}

void bus_configure(address adr)
{
    if (!bus_info.hdw_cfg) {
	bus_info.hdw_base = adr & 0xFFFC0;
	bus_info.hdw_cfg = TRUE;
	hdw_seg = SEG_OF (adr);
    } else if (!bus_info.ram_sz_cfg) {
	bus_info.ram_size = adr & 0xFF000;
	bus_info.ram_sz_cfg = TRUE;
    } else if (!bus_info.ram_cfg) {
	bus_info.ram_base = adr & 0xFF000;
	bus_info.ram_cfg = TRUE;
	bus_remap();
    } else if (!bus_info.ce1_sz_cfg) {
	bus_info.ce1_size = adr & 0xFF000;
	bus_info.ce1_sz_cfg = TRUE;
    } else if (!bus_info.ce1_cfg) {
	bus_info.ce1_base = adr & 0xFF000;
	bus_info.ce1_cfg = TRUE;
	bus_remap();
    } else if (!bus_info.ce2_sz_cfg) {
	bus_info.ce2_size = adr & 0xFF000;
	bus_info.ce2_sz_cfg = TRUE;
    } else if (!bus_info.ce2_cfg) {
	bus_info.ce2_base = adr & 0xFF000;
	bus_info.ce2_cfg = TRUE;
	bus_remap();
    } else if (!bus_info.nce3_sz_cfg) {
	bus_info.nce3_size = adr & 0xFF000;
	bus_info.nce3_sz_cfg = TRUE;
    } else if (!bus_info.nce3_cfg) {
	bus_info.nce3_base = adr & 0xFF000;
	bus_info.nce3_cfg = TRUE;
	bus_remap();
    }
}

void bus_unconfigure(address adr)
{
    if (bus_info.hdw_cfg && ((adr ^ bus_info.hdw_base) & 0xFFFC0) == 0) {
	bus_info.hdw_cfg = FALSE;
	hdw_seg = -1;
    } else if (bus_info.ram_cfg && ((adr ^ bus_info.ram_base) & bus_info.ram_size) == 0) {
	bus_info.ram_cfg = FALSE;
	bus_info.ram_sz_cfg = FALSE;
	bus_remap();
    } else if (bus_info.ce2_cfg && ((adr ^ bus_info.ce2_base) & bus_info.ce2_size) == 0) {
	bus_info.ce2_cfg = FALSE;
	bus_info.ce2_sz_cfg = FALSE;
	bus_remap();
    } else if (bus_info.ce1_cfg && ((adr ^ bus_info.ce1_base) & bus_info.ce1_size) == 0) {
	bus_info.ce1_cfg = FALSE;
	bus_info.ce1_sz_cfg = FALSE;
	bus_remap();
    } else if (bus_info.nce3_cfg && ((adr ^ bus_info.nce3_base) & bus_info.nce3_size) == 0) {
	bus_info.nce3_cfg = FALSE;
	bus_info.nce3_sz_cfg = FALSE;
	bus_remap();
    }
}

void bus_reset(void)
{
    bus_info.hdw_base = 0x00000;
    bus_info.hdw_cfg = FALSE;

    bus_info.ram_base = 0x00000;
    bus_info.ram_size = 0x00000;
    bus_info.ram_cfg = FALSE;
    bus_info.ram_sz_cfg = FALSE;

    bus_info.ce1_base = 0x00000;
    bus_info.ce1_size = 0x00000;
    bus_info.ce1_cfg = FALSE;
    bus_info.ce1_sz_cfg = FALSE;

    bus_info.ce2_base = 0x00000;
    bus_info.ce2_size = 0x00000;
    bus_info.ce2_cfg = FALSE;
    bus_info.ce2_sz_cfg = FALSE;

    bus_info.nce3_base = 0x00000;
    bus_info.nce3_size = 0x00000;
    bus_info.nce3_cfg = FALSE;
    bus_info.nce3_sz_cfg = FALSE;

    hdw_seg = -1;
    bus_remap();
}

address bus_get_id(void)
{
    if (!bus_info.hdw_cfg) {
	return bus_info.hdw_base | 0x00019;
    } else if (!bus_info.ram_sz_cfg) {
	return bus_info.ram_size | 0x00003;
    } else if (!bus_info.ram_cfg) {
	return bus_info.ram_base | 0x000F4;
    } else if (!bus_info.ce1_sz_cfg) {
	return bus_info.ce1_size | 0x00005;
    } else if (!bus_info.ce1_cfg) {
	return bus_info.ce1_base | 0x000F6;
    } else if (!bus_info.ce2_sz_cfg) {
	return bus_info.ce2_size | 0x00007;
    } else if (!bus_info.ce2_cfg) {
	return bus_info.ce2_base | 0x000F8;
    } else if (!bus_info.nce3_sz_cfg) {
	return bus_info.nce3_size | 0x00001;
    } else if (!bus_info.nce3_cfg) {
	return bus_info.nce3_base | 0x000F2;
    } else {
	return 0x00000;
    }
}
