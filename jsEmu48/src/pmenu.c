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
#include "emulator.h"
#include "color.h"
#include "gui.h"
#include "pmenu.h"

static void exit_up(boolean action);
static void debug_up(boolean action);
static void files_up(boolean action);
static void about_up(boolean action);

static Button menu_buttons[] = {
    { 0,	0,    0,	79, 20,	BUTTON_B1RELEASE,		    "Exit",	NULL,	exit_up },
    { 1,	80,   0,	79, 20,	BUTTON_B1RELEASE,		    "Debug",	NULL,	debug_up },
    { 2,	160,  0,	79, 20,	BUTTON_B1RELEASE,		    "Files",	NULL,	files_up },
    { 3,	240,  0,	79, 20,	BUTTON_DISABLED | BUTTON_B1RELEASE, "About",	NULL,	about_up },
    { 4,	0,    0,	0,  0,	0,				    NULL,	NULL,	NULL }
};

enum MenuButtons {
    MENU_EXIT,
    MENU_DEBUG,
    MENU_FILES,
    MENU_ABOUT,
    MENU_COUNT
};

//static BITMAP *menu_bmp;

static void exit_up(boolean action)
{
    if (action) {
	please_exit = TRUE;
    }
}

static void debug_up(boolean action)
{
//    if (action) {
//	menu_buttons[MENU_DEBUG].flags |= BUTTON_DISABLED;
//	menu_buttons[MENU_FILES].flags &= ~BUTTON_DISABLED;
//	menu_buttons[MENU_ABOUT].flags &= ~BUTTON_DISABLED;
//	button_draw_all(menu_bmp, menu_buttons);
//	gui_hide_panel(PANEL_FILES);
//	gui_hide_panel(PANEL_ABOUT);
//	gui_show_panel(PANEL_DEBUG);
//    }
}

static void files_up(boolean action)
{
//    if (action) {
//	menu_buttons[MENU_DEBUG].flags &= ~BUTTON_DISABLED;
//	menu_buttons[MENU_FILES].flags |= BUTTON_DISABLED;
//	menu_buttons[MENU_ABOUT].flags &= ~BUTTON_DISABLED;
//	button_draw_all(menu_bmp, menu_buttons);
//	gui_hide_panel(PANEL_DEBUG);
//	gui_hide_panel(PANEL_ABOUT);
//	gui_show_panel(PANEL_FILES);
//    }
}

static void about_up(boolean action)
{
//    if (action) {
//	menu_buttons[MENU_DEBUG].flags &= ~BUTTON_DISABLED;
//	menu_buttons[MENU_FILES].flags &= ~BUTTON_DISABLED;
//	menu_buttons[MENU_ABOUT].flags |= BUTTON_DISABLED;
//	button_draw_all(menu_bmp, menu_buttons);
//	gui_hide_panel(PANEL_DEBUG);
//	gui_hide_panel(PANEL_FILES);
//	gui_show_panel(PANEL_ABOUT);
//    }
}

/*
void pmenu_show(BITMAP *bmp)
{
    menu_bmp = bmp;

    clear_to_color(menu_bmp, color[C_PANEL_BACK]);
    button_draw_all(menu_bmp, menu_buttons);
}
*/

void pmenu_hide(void)
{
    //menu_bmp = NULL;
}

void pmenu_down(int mx, int my, int mb)
{
    //button_mouse_down(menu_bmp, menu_buttons, mx, my, mb);
}

void pmenu_up(int mx, int my, int mb)
{
    //button_mouse_up(menu_bmp, menu_buttons, mx, my, mb);
}
