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

#ifndef __COLOR_H
#define __COLOR_H

enum LCD_Modes {
    LCD_MODE_SIMPLE,
    LCD_MODE_GRAY4,
    LCD_MODE_GRAY8,
    LCD_MODE_EXP
};

enum Colors {
    C_BACKGROUND,
    C_PANEL_BACK,
    C_PANEL_BORDER,
    C_PANEL_TEXT,
    C_PANEL_DISABLED,
    C_BUTTON_BACK,
    C_BUTTON_BORDER,
    C_BUTTON_PUSHED,
    C_BUTTON_TEXT,
    C_BUTTON_DISABLED,
    C_COUNT
};

extern int color[C_COUNT];

void color_lcd(int r0, int g0, int b0, int r1, int g1, int b1);
void color_lcd_mode(int mode);
void color_set_emu(int i, int r, int g, int b);
void color_init(void);

#endif
