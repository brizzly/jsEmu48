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

#ifndef __GUI_H
#define __GUI_H


#define SDL_TTF


#include "types.h"

enum Panels {
    PANEL_MENU,
    PANEL_CALC,
    PANEL_DEBUG,
    PANEL_FILES,
    PANEL_ABOUT,
    PANEL_COUNT
};

typedef struct {
	int index;
    int x, y;
    int w, h;
    int flags;
    char *text;
	char *textB;
	char *textC;
	char *textD;
    void (*down)(void);
    void (*up)(boolean action);
} Button;

/* Button flags:
 * Use BUTTON_B1RELEASE for normal buttons.
 * Use BUTTON_B1RELEASE | BUTTON_B2TOGGLE for calculator buttons.
 * Use BUTTON_B1TOGGLE for toggle buttons
 */
#define BUTTON_PUSHED	    0x01	// Set if button is pushed
#define BUTTON_DISABLED	    0x02	// If set the button will be grayed out
#define BUTTON_B1TOGGLE	    0x04	// Mouse button 1 toggles this button
#define BUTTON_B2TOGGLE	    0x08	// Mouse button 2 toggles this button
#define BUTTON_B1RELEASE    0x10	// Releaseing mouse button 1 anywhere unpushes the button

void gui_initKeyboard(Button * calcbuttons);
void gui_init(void);
void gui_exit(void);
void gui_update(void);
void gui_show_panel(int i);
void gui_hide_panel(int i);

/*
void button_draw(BITMAP *bmp, Button *buttons);
*/
void button_draw_all(/*BITMAP *bmp,*/ Button *buttons);
int button_mouse_down(/*BITMAP *bmp,*/ Button *butons, int mx, int my, int mb);
int button_mouse_up(/*BITMAP *bmp,*/ Button *buttons, int mx, int my, int mb);

#endif
