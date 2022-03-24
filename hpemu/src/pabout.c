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
#include "gui.h"
#include "pabout.h"

//static BITMAP *about_bmp;

static char *about_line[] = {
    "C   /                      ",
    "C  /__  ___  ___  ____     ",
    "C /  / /  / /__/ / / / /  /",
    "C/  / /__/ /__  /   / /__/ ",
    "C    /                     ",
    "C   /    version 0.9.0     ",
    "P",
    " Copyright 2002 Daniel Nilsson",
    "P",
    "JHpemu is free software; you can",
    "Jredistribute it and/or modify it under",
    "Jthe terms of the GNU General Public",
    "JLicense as published by the Free Software",
    "JFoundation; either version 2 of the",
    "JLicense, or (at your option) any later",
    "Jversion.",
    "P",
    "JHpemu is distributed in the hope that it",
    "Jwill be useful, but WITHOUT ANY WARRANTY;",
    "Jwithout even the implied warranty of",
    "JMERCHANTABILITY or FITNESS FOR A",
    "JPARTICULAR PURPOSE.  See the GNU General",
    "JPublic License for more details.",
    "P",
    "JYou should have received a copy of the",
    "JGNU General Public License along with",
    "Jhpemu; if not, write to the Free Software",
    "JFoundation, Inc., 59 Temple Place,",
    "JSuite 330, Boston, MA  02111-1307  USA",
    NULL
};

static void draw_about(void)
{
	/*
    int i, y;

    text_mode(color[C_PANEL_BACK]);

    acquire_bitmap(about_bmp);
    scare_mouse();

    y = 10;
    for (i = 0; about_line[i]; i++) {
	switch (about_line[i][0]) {
	case 'C':
	    textout_centre(about_bmp, font, about_line[i]+1, 170, y, color[C_PANEL_TEXT]);
	    break;
	case 'J':
	    textout_justify(about_bmp, font, about_line[i]+1, 10, 330, y, 150, color[C_PANEL_TEXT]);
	    break;
	case 'P':
	    y += 5;
	    break;
	default:
	    textout(about_bmp, font, about_line[i]+1, 10, y, color[C_PANEL_TEXT]);
	    break;
	}
	y += 10;
    }
    
    unscare_mouse();
    release_bitmap(about_bmp);*/
}

/*
void pabout_show(BITMAP *bmp)
{
    about_bmp = bmp;

    clear_to_color(about_bmp, color[C_PANEL_BACK]);
    draw_about();
}
*/

void pabout_hide(void)
{
    //about_bmp = NULL;
}

void pabout_down(int mx, int my, int mb)
{
}

void pabout_up(int mx, int my, int mb)
{
}
