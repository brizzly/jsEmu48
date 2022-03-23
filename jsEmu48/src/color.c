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
#include "color.h"

#define RESERVED_LCD	128

int color[C_COUNT];

typedef struct RGB
{
	unsigned char r, g, b;
} RGB;

//static PALETTE palette;

static int lcd_0_r, lcd_0_g, lcd_0_b;
static int lcd_1_r, lcd_1_g, lcd_1_b;
static int lcd_mode;

static void set_lcd_color(int i, int v)
{
//    palette[i].r = (lcd_0_r * (255 - v) + lcd_1_r * v) / 255;
//    palette[i].g = (lcd_0_g * (255 - v) + lcd_1_g * v) / 255;
//    palette[i].b = (lcd_0_b * (255 - v) + lcd_1_b * v) / 255;
}

static int bit_count(unsigned int i)
{
    int n = 0;

    while (i) {
	n += i & 1;
	i >>= 1;
    }
    return n;
}

static int simple_color(int i)
{
    return (i & 0x40) ? 255 : 0;
}

static int gray4_color(int i)
{
    return bit_count(i & 0x70) * 85;
}

static int gray8_color(int i)
{
    return bit_count(i) * 255 / 7;
}

static int exp_color(int i)
{
    return i * 255 / 127;
}

typedef int (*lcd_color_func)(int i);

lcd_color_func lcd_color_functions[] = {
    simple_color,
    gray4_color,
    gray8_color,
    exp_color
};

void build_lcd_palette(void)
{
	/*
    int i;

    for (i = 0; i < RESERVED_LCD; i++) {
		set_lcd_color(i, lcd_color_functions[lcd_mode](i));
    }
	 */
  //  set_palette_range(palette, 0, RESERVED_LCD-1, FALSE);
}

void color_lcd(int r0, int g0, int b0, int r1, int g1, int b1)
{
    lcd_0_r = r0 >> 2;
    lcd_0_g = g0 >> 2;
    lcd_0_b = b0 >> 2;
    lcd_1_r = r1 >> 2;
    lcd_1_g = g1 >> 2;
    lcd_1_b = b1 >> 2;
    build_lcd_palette();
}

void color_lcd_mode(int mode)
{
    lcd_mode = mode;
    build_lcd_palette();
}

void color_set_emu(int i, int r, int g, int b)
{
//    if (bitmap_color_depth(screen) == 8) {
//	palette[color[i]].r = r >> 2;
//	palette[color[i]].g = g >> 2;
//	palette[color[i]].b = b >> 2;
//	set_color(color[i], palette + color[i]);
//    } else {
//	color[i] = makecol(r, g, b);
//    }
}

void color_init(void)
{
    int i;

    //if (bitmap_color_depth(screen) == 8) {
	for (i = 0; i < C_COUNT; i++) {
	    color[i] = RESERVED_LCD + i;
	}
    //}
	
    color_set_emu(C_BACKGROUND,	0,	0,	0);
    color_set_emu(C_PANEL_BACK,	64,	64,	64);
    color_set_emu(C_PANEL_BORDER,	128,	128,	128);
    color_set_emu(C_PANEL_TEXT,	255,	255,	255);
    color_set_emu(C_PANEL_DISABLED,	128,	128,	128);
    color_set_emu(C_BUTTON_BACK,	64,	64,	64);
    color_set_emu(C_BUTTON_BORDER,	128,	128,	128);
    color_set_emu(C_BUTTON_PUSHED,	128,	128,	128);
    color_set_emu(C_BUTTON_TEXT,	255,	255,	255);
    color_set_emu(C_BUTTON_DISABLED,128,	128,	128);

    color_lcd(128, 192, 128, 0, 0, 64);
    color_lcd_mode(LCD_MODE_GRAY4);
    //set_palette(palette);
    //set_mouse_sprite(NULL);
    //gui_bg_color = color[C_PANEL_BACK];
    //gui_mg_color = color[C_PANEL_DISABLED];
    //gui_fg_color = color[C_PANEL_TEXT];
}
