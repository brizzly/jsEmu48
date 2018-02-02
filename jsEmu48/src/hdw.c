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
#include "bus.h"
#include "ports.h"
#include "timers.h"
#include "display.h"
#include "hdw.h"

static byte hdw_ram[64];

void hdw_init(void)
{
}

void hdw_exit(void)
{
}

byte hdw_read_nibble(address adr)
{
    switch (adr) {

    case 0x00:	return display_offset | (display_enable ? 0x8 : 0x0);

    case 0x04:	return (byte) crc & 0xF;
    case 0x05:	return (byte) (crc >> 4) & 0xF;
    case 0x06:	return (byte) (crc >> 8) & 0xF;
    case 0x07:	return (byte) (crc >> 12) & 0xF;

    case 0x0F:	return (hdw_ram[0x0E] & 0x8) ? ports_card_detect() : 0;

    case 0x28:	return display_line_count & 0xF;
    case 0x29:	return (display_line_count>>4) | (hdw_ram[0x29] & 0x4) | (bus_info.da19 ? 0x8 : 0x0);

    case 0x2E:	return timer1_control;
    case 0x2F:	return timer2_control;

    case 0x37:	return timer1_value;
    case 0x38:	return (byte) timer2_value & 0xF;
    case 0x39:	return (byte) (timer2_value >> 4) & 0xF;
    case 0x3A:	return (byte) (timer2_value >> 8) & 0xF;
    case 0x3B:	return (byte) (timer2_value >> 12) & 0xF;
    case 0x3C:	return (byte) (timer2_value >> 16) & 0xF;
    case 0x3D:	return (byte) (timer2_value >> 20) & 0xF;
    case 0x3E:	return (byte) (timer2_value >> 24) & 0xF;
    case 0x3F:	return (byte) (timer2_value >> 28) & 0xF;

    default:	return hdw_ram[adr];

    }
}

void hdw_write_nibble(byte data, address adr)
{
    int tmp;

    switch (adr) {

    case 0x00:	display_offset = data & 7;
		display_enable = (data & 8) ? TRUE : FALSE;
		break;

    case 0x04:	crc &= 0xFFF0;
		crc |= (word) data;
		break;
    case 0x05:	crc &= 0xFF0F;
		crc |= (word) data << 4;
		break;
    case 0x06:	crc &= 0xF0FF;
		crc |= (word) data << 8;
		break;
    case 0x07:	crc &= 0x0FFF;
		crc |= (word) data << 12;
		break;

    case 0x0F:	break;

    case 0x20:	display_base &= 0xFFFF0;
		display_base |= (address) data & 0xE;
		break;
    case 0x21:	display_base &= 0xFFF0F;
		display_base |= (address) data << 4;
		break;
    case 0x22:	display_base &= 0xFF0FF;
		display_base |= (address) data << 8;
		break;
    case 0x23:	display_base &= 0xF0FFF;
		display_base |= (address) data << 12;
		break;
    case 0x24:	display_base &= 0x0FFFF;
		display_base |= (address) data << 16;
		break;
    case 0x25:	display_line_offset &= 0xFF0;
		display_line_offset |= (address) data & 0xE;
		break;
    case 0x26:	display_line_offset &= 0xF0F;
		display_line_offset |= (address) data << 4;
		break;
    case 0x27:	display_line_offset &= 0x0FF;
		display_line_offset |= (address) data << 8;
		if (display_line_offset & 0x800)
		    display_line_offset |= ~0xFFF;
		else
		    display_line_offset &= 0xFFF;
		break;
    case 0x28:	display_height &= 0xF0;
		display_height |= data;
		break;
    case 0x29:	display_height &= 0x0F;
		display_height |= (data&3) << 4;
		hdw_ram[0x29] = data & 0x4;
		tmp = bus_info.da19;
		bus_info.da19 = (data & 0x8) ? TRUE : FALSE;
		if (tmp != bus_info.da19) {
		    bus_remap();
		}
		break;

    case 0x2E:	timer1_control = data; break;
    case 0x2F:	timer2_control = data; break;

    case 0x30:	menu_base &= 0xFFFF0;
		menu_base |= (address) data & 0xE;
		break;
    case 0x31:	menu_base &= 0xFFF0F;
		menu_base |= (address) data << 4;
		break;
    case 0x32:	menu_base &= 0xFF0FF;
		menu_base |= (address) data << 8;
		break;
    case 0x33:	menu_base &= 0xF0FFF;
		menu_base |= (address) data << 12;
		break;
    case 0x34:	menu_base &= 0x0FFFF;
		menu_base |= (address) data << 16;
		break;

    case 0x37:	timer1_value = data; break;
    case 0x38:	timer2_value &= 0xFFFFFFF0;
		timer2_value |= (dword) data;
		break;
    case 0x39:	timer2_value &= 0xFFFFFF0F;
		timer2_value |= (dword) data << 4;
		break;
    case 0x3A:	timer2_value &= 0xFFFFF0FF;
		timer2_value |= (dword) data << 8;
		break;
    case 0x3B:	timer2_value &= 0xFFFF0FFF;
		timer2_value |= (dword) data << 12;
		break;
    case 0x3C:	timer2_value &= 0xFFF0FFFF;
		timer2_value |= (dword) data << 16;
		break;
    case 0x3D:	timer2_value &= 0xFF0FFFFF;
		timer2_value |= (dword) data << 20;
		break;
    case 0x3E:	timer2_value &= 0xF0FFFFFF;
		timer2_value |= (dword) data << 24;
		break;
    case 0x3F:	timer2_value &= 0x0FFFFFFF;
		timer2_value |= (dword) data << 28;
		break;

    default:	hdw_ram[adr] = data;
		break;
    }
}
