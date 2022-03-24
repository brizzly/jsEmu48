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

//#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "emulator.h"
#include "cpu.h"
#include "bus.h"
#include "disasm.h"
#include "color.h"
#include "gui.h"
#include "pdebug.h"

static void run_up(boolean action);
static void break_up(boolean action);
static void step_up(boolean action);

static Button debug_buttons[] = {
    { 0,	0,    0,	79, 20,	BUTTON_B1RELEASE,   "Break",	NULL,	break_up },
    { 1,	80,   0,	79, 20,	BUTTON_B1RELEASE,   "Run",	NULL,	run_up },
    { 2,	160,  0,	79, 20,	BUTTON_B1RELEASE,   "Step",	NULL,	step_up },
    { 3,	0,    0,	0,  0,	0,		    NULL,	NULL,	NULL }
};

#define BREAK_BUTTON	0
#define RUN_BUTTON	1
#define STEP_BUTTON	2

//static BITMAP *debug_bmp;

static void update_buttons(void)
{
	/*
    if (emulator_get_state() == EMULATOR_STOP) {
	if (debug_buttons[RUN_BUTTON].flags & BUTTON_DISABLED) {
	    debug_buttons[RUN_BUTTON].flags &= ~BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + RUN_BUTTON);
	}
	if (!(debug_buttons[BREAK_BUTTON].flags & BUTTON_DISABLED)) {
	    debug_buttons[BREAK_BUTTON].flags |= BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + BREAK_BUTTON);
	}
	if (debug_buttons[STEP_BUTTON].flags & BUTTON_DISABLED) {
	    debug_buttons[STEP_BUTTON].flags &= ~BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + STEP_BUTTON);
	}
    } else {
	if (!(debug_buttons[RUN_BUTTON].flags & BUTTON_DISABLED)) {
	    debug_buttons[RUN_BUTTON].flags |= BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + RUN_BUTTON);
	}
	if (debug_buttons[BREAK_BUTTON].flags & BUTTON_DISABLED) {
	    debug_buttons[BREAK_BUTTON].flags &= ~BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + BREAK_BUTTON);
	}
	if (!(debug_buttons[STEP_BUTTON].flags & BUTTON_DISABLED)) {
	    debug_buttons[STEP_BUTTON].flags |= BUTTON_DISABLED;
	    if (debug_bmp) button_draw(debug_bmp, debug_buttons + STEP_BUTTON);
	}
    }*/
}

static void run_up(boolean action)
{
    if (action) {
		emulator_set_state(EMULATOR_RUN);
    }
}

static void break_up(boolean action)
{
    if (action) {
		emulator_set_state(EMULATOR_STOP);
    }
}

static void step_up(boolean action)
{
    if (action) {
		emulator_set_state(EMULATOR_STEP);
    }
}

/*static*/ void draw_cpu(void)
{
    int c1 = color[C_PANEL_TEXT];
    int c2 = color[C_PANEL_DISABLED];
    int i;
    char *tmp;

//    if (!debug_bmp) {
//	return;
//    }

    //text_mode(color[C_PANEL_BACK]);

    //acquire_bitmap(debug_bmp);
    //scare_mouse();

    tmp = disassemble(bus_fast_peek(NULL, cpu.pc, NULL));
	
	printf("PC = #%05X   %-24s\n", (int) cpu.pc, tmp);
	
	/*
    printf("A  = #%s ", nib_to_hex_rev(cpu.reg[A], 16));
    printf("B  = #%s ", nib_to_hex_rev(cpu.reg[B], 16));
    printf("C  = #%s ", nib_to_hex_rev(cpu.reg[C], 16));
    printf("D  = #%s ", nib_to_hex_rev(cpu.reg[D], 16));
	printf("\n");
    //printf("%*c%*c ", (int) 16-cpu.p, 'P', (int) cpu.p+1, '\0');
	//printf("\n");
    printf("R0 = #%s ", nib_to_hex_rev(cpu.reg_r[0], 16));
    printf("R1 = #%s ", nib_to_hex_rev(cpu.reg_r[1], 16));
    printf("R2 = #%s ", nib_to_hex_rev(cpu.reg_r[2], 16));
    printf("R3 = #%s ", nib_to_hex_rev(cpu.reg_r[3], 16));
    printf("R4 = #%s ", nib_to_hex_rev(cpu.reg_r[4], 16));
	printf("\n");

    tmp = nib_to_hex(bus_fast_peek(NULL, cpu.d[0], NULL), 16);
    printf("D0 = #%05X   (%s)\n", (int) cpu.d[0], tmp);
    tmp = nib_to_hex(bus_fast_peek(NULL, cpu.d[1], NULL), 16);
    printf("D1 = #%05X   (%s)\n", (int) cpu.d[1], tmp);

    //printf("RSTK", 10, 190, c1);
    for (i = 0; i < 8; i++) {
		printf("    #%05X\n", (int) cpu.rstk[(cpu.rstk_ptr+i)&7]);
    }
	printf("\n");
	

    printf("P = #%X ", (int) cpu.p);
 
    printf("OUT = #%s ", nib_to_hex_rev(cpu.out, 3));
    printf("IN = #%s ", nib_to_hex_rev(cpu.in, 4));
    printf("ST = #%s ", nib_to_hex_rev(cpu.st, 4));
	
	printf("\n");
	*/
	

	/*
    textout(debug_bmp, font, "MP", 200, 120, (cpu.hst & 0x8) ? c1 : c2);
    textout(debug_bmp, font, "SR", 224, 120, (cpu.hst & 0x4) ? c1 : c2);
    textout(debug_bmp, font, "SB", 248, 120, (cpu.hst & 0x2) ? c1 : c2);
    textout(debug_bmp, font, "XM", 272, 120, (cpu.hst & 0x1) ? c1 : c2);

    textout(debug_bmp, font, "CARRY", 200, 130, cpu.carry ? c1 : c2);
    textout(debug_bmp, font, cpu.dec ? "DEC" : "HEX", 200, 140, c1);

    textout(debug_bmp, font, "SHUTDOWN", 10, 280, cpu.shutdown ? c1 : c2);
    textout(debug_bmp, font, "KEYSCAN",  90, 280, cpu.keyscan ? c1 : c2);
    textout(debug_bmp, font, "INTE",    170, 280, cpu.inte ? c1 : c2);
    textout(debug_bmp, font, "KEYINTP", 250, 280, cpu.keyintp ? c1 : c2);

    textout(debug_bmp, font, "HDWREG  RAM    CE1    CE2    NCE3", 58, 310, c1);
    textout(debug_bmp, font, "Mask", 10, 320, c1);
    textout(debug_bmp, font, "-----", 58, 320, c1);
    if (bus_info.ram_sz_cfg) {
		textprintf(debug_bmp, font, 114, 320, c1, "%05X", bus_info.ram_size);
    } else {
		textout(debug_bmp, font, "-----", 114, 320, c1);
    }
    if (bus_info.ce1_sz_cfg) {
		textprintf(debug_bmp, font, 170, 320, c1, "%05X", bus_info.ce1_size);
    } else {
		textout(debug_bmp, font, "-----", 170, 320, c1);
    }
    if (bus_info.ce2_sz_cfg) {
		textprintf(debug_bmp, font, 226, 320, c1, "%05X", bus_info.ce2_size);
    } else {
		textout(debug_bmp, font, "-----", 226, 320, c1);
    }
    if (bus_info.nce3_sz_cfg) {
		textprintf(debug_bmp, font, 282, 320, c1, "%05X", bus_info.nce3_size);
    } else {
	textout(debug_bmp, font, "-----", 282, 320, c1);
    }
    textout(debug_bmp, font, "Base", 10, 330, c1);
    if (bus_info.hdw_cfg) {
	textprintf(debug_bmp, font, 58, 330, c1, "%05X", bus_info.hdw_base);
    } else {
	textout(debug_bmp, font, "-----", 58, 330, c1);
    }
    if (bus_info.ram_cfg) {
	textprintf(debug_bmp, font, 114, 330, c1, "%05X", bus_info.ram_base);
    } else {
	textout(debug_bmp, font, "-----", 114, 330, c1);
    }
    if (bus_info.ce1_cfg) {
	textprintf(debug_bmp, font, 170, 330, c1, "%05X", bus_info.ce1_base);
    } else {
	textout(debug_bmp, font, "-----", 170, 330, c1);
    }
    if (bus_info.ce2_cfg) {
	textprintf(debug_bmp, font, 226, 330, c1, "%05X", bus_info.ce2_base);
    } else {
	textout(debug_bmp, font, "-----", 226, 330, c1);
    }
    if (bus_info.nce3_cfg) {
	textprintf(debug_bmp, font, 282, 330, c1, "%05X", bus_info.nce3_base);
    } else {
	textout(debug_bmp, font, "-----", 282, 330, c1);
    }

    textprintf(debug_bmp, font, 10, 380, c1, "Instruction count = %u", cpu.inst_cnt);
    textprintf(debug_bmp, font, 10, 390, c1, "Cycle count = %u", cpu.cycles);
*/
//    unscare_mouse();
//    release_bitmap(debug_bmp);
}

void pdebug_draw_true_speed(dword speed)
{
	 //printf("True speed: %10u Hz\n", speed);
	
//    if (!debug_bmp) {
//	return;
//    }
//    text_mode(color[C_PANEL_BACK]);
//    acquire_bitmap(debug_bmp);
//    scare_mouse();
//    textprintf(debug_bmp, font, 10, 410, color[C_PANEL_TEXT], "True speed: %10u Hz", speed);
//    unscare_mouse();
//    release_bitmap(debug_bmp);
}

void pdebug_state_changed(void)
{
    update_buttons();
    draw_cpu();
}

/*
void pdebug_show(BITMAP *bmp)
{
    update_buttons();

    debug_bmp = bmp;

    clear_to_color(debug_bmp, color[C_PANEL_BACK]);
    button_draw_all(debug_bmp, debug_buttons);
    draw_cpu();
}
*/

void pdebug_hide(void)
{
    //debug_bmp = NULL;
}

void pdebug_down(int mx, int my, int mb)
{
    //button_mouse_down(debug_bmp, debug_buttons, mx, my, mb);
}

void pdebug_up(int mx, int my, int mb)
{
    //button_mouse_up(debug_bmp, debug_buttons, mx, my, mb);
}
