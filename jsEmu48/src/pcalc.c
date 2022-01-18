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

#include <stdlib.h>
#include "color.h"
#include "display.h"
#include "keyboard.h"
#include "gui.h"
#include "pcalc.h"
#include "pfiles.h"
//#include "emscripten.h"


static void dn00(void)			{ kbd_key_pressed  (0, 0); }
static void up00(boolean action)	{ kbd_key_released (0, 0); }
static void dn01(void)			{ kbd_key_pressed  (0, 1); }
static void up01(boolean action)	{ kbd_key_released (0, 1); }
static void dn02(void)			{ kbd_key_pressed  (0, 2); }
static void up02(boolean action)	{ kbd_key_released (0, 2); }
static void dn03(void)			{ kbd_key_pressed  (0, 3); }
static void up03(boolean action)	{ kbd_key_released (0, 3); }
static void dn04(void)			{ kbd_key_pressed  (0, 4); }
static void up04(boolean action)	{ kbd_key_released (0, 4); }
static void dn10(void)			{ kbd_key_pressed  (1, 0); }
static void up10(boolean action)	{ kbd_key_released (1, 0); }
static void dn11(void)			{ kbd_key_pressed  (1, 1); }
static void up11(boolean action)	{ kbd_key_released (1, 1); }
static void dn12(void)			{ kbd_key_pressed  (1, 2); }
static void up12(boolean action)	{ kbd_key_released (1, 2); }
static void dn13(void)			{ kbd_key_pressed  (1, 3); }
static void up13(boolean action)	{ kbd_key_released (1, 3); }
static void dn14(void)			{ kbd_key_pressed  (1, 4); }
static void up14(boolean action)	{ kbd_key_released (1, 4); }
static void dn15(void)			{ kbd_key_pressed  (1, 5); }
static void up15(boolean action)	{ kbd_key_released (1, 5); }
static void dn20(void)			{ kbd_key_pressed  (2, 0); }
static void up20(boolean action)	{ kbd_key_released (2, 0); }
static void dn21(void)			{ kbd_key_pressed  (2, 1); }
static void up21(boolean action)	{ kbd_key_released (2, 1); }
static void dn22(void)			{ kbd_key_pressed  (2, 2); }
static void up22(boolean action)	{ kbd_key_released (2, 2); }
static void dn23(void)			{ kbd_key_pressed  (2, 3); }
static void up23(boolean action)	{ kbd_key_released (2, 3); }
static void dn24(void)			{ kbd_key_pressed  (2, 4); }
static void up24(boolean action)	{ kbd_key_released (2, 4); }
static void dn25(void)			{ kbd_key_pressed  (2, 5); }
static void up25(boolean action)	{ kbd_key_released (2, 5); }
static void dn30(void)			{ kbd_key_pressed  (3, 0); }
static void up30(boolean action)	{ kbd_key_released (3, 0); }
static void dn31(void)			{ kbd_key_pressed  (3, 1); }
static void up31(boolean action)	{ kbd_key_released (3, 1); }
static void dn32(void)			{ kbd_key_pressed  (3, 2); }
static void up32(boolean action)	{ kbd_key_released (3, 2); }
static void dn33(void)			{ kbd_key_pressed  (3, 3); }
static void up33(boolean action)	{ kbd_key_released (3, 3); }
static void dn34(void)			{ kbd_key_pressed  (3, 4); }
static void up34(boolean action)	{ kbd_key_released (3, 4); }
static void dn35(void)			{ kbd_key_pressed  (3, 5); }
static void up35(boolean action)	{ kbd_key_released (3, 5); }
static void dn40(void)			{ kbd_key_pressed  (4, 0); }
static void up40(boolean action)	{ kbd_key_released (4, 0); }
static void dn41(void)			{ kbd_key_pressed  (4, 1); }
static void up41(boolean action)	{ kbd_key_released (4, 1); }
static void dn42(void)			{ kbd_key_pressed  (4, 2); }
static void up42(boolean action)	{ kbd_key_released (4, 2); }
static void dn43(void)			{ kbd_key_pressed  (4, 3); }
static void up43(boolean action)	{ kbd_key_released (4, 3); }
static void dn44(void)			{ kbd_key_pressed  (4, 4); }
static void up44(boolean action)	{ kbd_key_released (4, 4); }
static void dn50(void)			{ kbd_key_pressed  (5, 0); }
static void up50(boolean action)	{ kbd_key_released (5, 0); }
static void dn51(void)			{ kbd_key_pressed  (5, 1); }
static void up51(boolean action)	{ kbd_key_released (5, 1); }
static void dn52(void)			{ kbd_key_pressed  (5, 2); }
static void up52(boolean action)	{ kbd_key_released (5, 2); }
static void dn53(void)			{ kbd_key_pressed  (5, 3); }
static void up53(boolean action)	{ kbd_key_released (5, 3); }
static void dn54(void)			{ kbd_key_pressed  (5, 4); }
static void up54(boolean action)	{ kbd_key_released (5, 4); }
static void dn60(void)			{ kbd_key_pressed  (6, 0); }
static void up60(boolean action)	{ kbd_key_released (6, 0); }
static void dn61(void)			{ kbd_key_pressed  (6, 1); }
static void up61(boolean action)	{ kbd_key_released (6, 1); }
static void dn62(void)			{ kbd_key_pressed  (6, 2); }
static void up62(boolean action)	{ kbd_key_released (6, 2); }
static void dn63(void)			{ kbd_key_pressed  (6, 3); }
static void up63(boolean action)	{ kbd_key_released (6, 3); }
static void dn64(void)			{ kbd_key_pressed  (6, 4); }
static void up64(boolean action)	{ kbd_key_released (6, 4); }
static void dn70(void)			{ kbd_key_pressed  (7, 0); }
static void up70(boolean action)	{ kbd_key_released (7, 0); }
static void dn71(void)			{ kbd_key_pressed  (7, 1); }
static void up71(boolean action)	{ kbd_key_released (7, 1); }
static void dn72(void)			{ kbd_key_pressed  (7, 2); }
static void up72(boolean action)	{ kbd_key_released (7, 2); }
static void dn73(void)			{ kbd_key_pressed  (7, 3); }
static void up73(boolean action)	{ kbd_key_released (7, 3); }
static void dn74(void)			{ kbd_key_pressed  (7, 4); }
static void up74(boolean action)	{ kbd_key_released (7, 4); }
static void dn80(void)			{ kbd_key_pressed  (8, 0); }
static void up80(boolean action) { kbd_key_released (8, 0); }
static void dn81(void)			{ kbd_key_pressed  (8, 1); }
static void up81(boolean action)	{ kbd_key_released (8, 1); }
static void dn82(void)			{ kbd_key_pressed  (8, 2); }
static void up82(boolean action)	{ kbd_key_released (8, 2); }
static void dn83(void)			{ kbd_key_pressed  (8, 3); }
static void up83(boolean action)	{ kbd_key_released (8, 3); }
static void dn84(void)			{ kbd_key_pressed  (8, 4); }
static void up84(boolean action)	{ kbd_key_released (8, 4); }
static void dnON(void)			{ kbd_on_pressed  (); }
static void upON(boolean action)	{ kbd_on_released (); }

static void dnZelda(void)			{  }
static void upZelda(boolean action)	{  load_file("zeldahp.dir"); }
//static void upZelda(boolean action)	{  load_file("Arkalite.lib"); }



const int pox = 2;
const int poy = 55;
const int pow1 = 40;
const int poh1 = 18;
const int poh2 = 22;
const int pow2 = 48;
const int ystart = 80;
const int yspacing = 14;
const int xstart = 5;
const int xspacing = (pow1+2);
const int xspacing2 = (pow2+2);
const int enter_w = pow1*2 + 2;

static KBMapping kb_sdl_mapping[] = {
	
	SDL_SCANCODE_KP_0, dn03, up03,
	SDL_SCANCODE_KP_1, dn13, up13,
	SDL_SCANCODE_KP_2, dn12, up12,
	SDL_SCANCODE_KP_3, dn11, up11,
	SDL_SCANCODE_KP_4, dn23, up23,
	SDL_SCANCODE_KP_5, dn22, up22,
	SDL_SCANCODE_KP_6, dn21, up21,
	SDL_SCANCODE_KP_7, dn33, up33,
	SDL_SCANCODE_KP_8, dn32, up32,
	SDL_SCANCODE_KP_9, dn31, up31
};

static Button calc_buttons[] = {
	
    { 0, 	pox+xstart+(xspacing*0),	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "A", dn14,	up14 },
    { 1,	pox+xstart+(xspacing*1),   	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "B", dn84,	up84 },
    { 2,	pox+xstart+(xspacing*2),   	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "C", dn83,	up83 },
    { 3,	pox+xstart+(xspacing*3),  	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "D", dn82,	up82 },
    { 4,	pox+xstart+(xspacing*4),  	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "E", dn81,	up81 },
    { 5,	pox+xstart+(xspacing*5),  	ystart+(0*yspacing)+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "", "F", dn80,	up80 },
	
	{ 6,	pox+xstart+(xspacing*0),    ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "MTH", "RAD", "POLAR", "G",	dn24,	up24 },
    { 7,	pox+xstart+(xspacing*1),   	ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "PRG", "", "CHARS", "H",	dn74,	up74 },
    { 8,	pox+xstart+(xspacing*2),   	ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "CST", "", "MODES", "I",	dn73,	up73 },
    { 9,	pox+xstart+(xspacing*3),  	ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "VAR", "", "MEMORY", "J",	dn72,	up72 },
    { 10,	pox+xstart+(xspacing*4),  	ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "^", "", "STACK", "K",	dn71,	up71 },
    { 11,	pox+xstart+(xspacing*5),  	ystart+(1*yspacing)+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "NXT", "PREV", "MENU", "L",	dn70,	up70 },
	
	{ 12,	pox+xstart+(xspacing*0),    ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "'", "UP", "HOME", "M",	dn04,	up04 },
    { 13,	pox+xstart+(xspacing*1),   	ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "STO", "REF", "RCL", "N",	dn64,	up64 },
    { 14, 	pox+xstart+(xspacing*2),   	ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "EVAL", "->NUM", "UNDO", "O",	dn63,	up63 },
    { 15,	pox+xstart+(xspacing*3),  	ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "<", "PICTURE", "", "P",	dn62,	up62 },
    { 16,	pox+xstart+(xspacing*4),  	ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "v", "VIEW", "", "Q",	dn61,	up61 },
    { 17,	pox+xstart+(xspacing*5),  	ystart+(2*yspacing)+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, ">", "SWAP", "", "R",	dn60,	up60 },
	
	{ 18,	pox+xstart+(xspacing*0),    ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "SIN", "ASIN", "tet", "S",	dn34,	up34 },
    { 19,	pox+xstart+(xspacing*1),   	ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "COS", "ACOS", "", "T",	dn54,	up54 },
    { 20,	pox+xstart+(xspacing*2),   	ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "TAN", "ATAN", "Sig", "U",	dn53,	up53 },
    { 21,	pox+xstart+(xspacing*3),  	ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "SQ x", "xx", "x SQ y", "V",	dn52,	up52 },
    { 22,	pox+xstart+(xspacing*4),  	ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "yx", "10x", "LOG", "W",	dn51,	up51 },
    { 23,	pox+xstart+(xspacing*5),  	ystart+(3*yspacing)+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "1/x", "ex", "LN", "X",	dn50,	up50 },
	
	{ 24,	pox+xstart+(xspacing*0),    ystart+(4*yspacing)+poy+40,    enter_w,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "ENTER", "EQUATION", "MATRIX", "", dn44,	up44 },
    { 25,	enter_w-pow1+pox+xstart+(xspacing*1),   ystart+(4*yspacing)+poy+40,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "+/-", "EDIT", "CMD", "Y",	dn43,	up43 },
    { 26,	enter_w-pow1+pox+xstart+(xspacing*2),  ystart+(4*yspacing)+poy+40,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "EEX", "PURG", "ARG", "Z",	dn42,	up42 },
    { 27,	enter_w-pow1+pox+xstart+(xspacing*3),  ystart+(4*yspacing)+poy+40,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "DEL", "CLEAR", "", "",	dn41,	up41 },
    { 28,	enter_w-pow1+pox+xstart+(xspacing*4),  ystart+(4*yspacing)+poy+40,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "<-", "DROP", "", "",	dn40,	up40 },
	
	{ 29,	pox+xstart+(xspacing*0),    ystart+(5*yspacing)+poy+50,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "alpha", "USER", "ENTRY", "",	dn35,	up35 },
    { 30,	pox+xstart+(xspacing2*1),   ystart+(5*yspacing)+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "7", "", "SOLVE", "",	dn33,	up33 },
    { 31,	pox+xstart+(xspacing2*2),  ystart+(5*yspacing)+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "8", "", "PLOT", "",	dn32,	up32 },
    { 32,	pox+xstart+(xspacing2*3),  ystart+(5*yspacing)+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "9", "", "SYMBOLIC", "",	dn31,	up31 },
    { 33,	pox+xstart+(xspacing2*4),  ystart+(5*yspacing)+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "/", "( )", "#", "",	dn30,	up30 },
	
	{ 34,	pox+xstart+(xspacing*0),    ystart+(6*yspacing)+poy+60,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "red", "", "",	dn25,	up25 },
    { 35,	pox+xstart+(xspacing2*1),   ystart+(6*yspacing)+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "4", "", "TIME", "",	dn23,	up23 },
    { 36,	pox+xstart+(xspacing2*2),  ystart+(6*yspacing)+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "5", "", "STAT", "",	dn22,	up22 },
    { 37,	pox+xstart+(xspacing2*3),  ystart+(6*yspacing)+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "6", "", "UNITS", "",	dn21,	up21 },
    { 38,	pox+xstart+(xspacing2*4),  ystart+(6*yspacing)+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "X", "[ ]", "_", "",	dn20,	up20 },
	
	{ 39,	pox+xstart+(xspacing*0),    ystart+(7*yspacing)+poy+70,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "", "", "green", "",	dn15,	up15 },
    { 40,	pox+xstart+(xspacing2*1),   ystart+(7*yspacing)+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "1", "", "I/O", "",	dn13,	up13 },
    { 41,	pox+xstart+(xspacing2*2),  ystart+(7*yspacing)+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "2", "", "LIBRARY", "",	dn12,	up12 },
    { 42,	pox+xstart+(xspacing2*3),  ystart+(7*yspacing)+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "3", "", "EQ LIB", "",	dn11,	up11 },
    { 43,	pox+xstart+(xspacing2*4),  ystart+(7*yspacing)+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "-", "<< >>", "\" \"", "",	dn10,	up10 },
	
	{ 44,	pox+xstart+(xspacing*0),    ystart+(8*yspacing)+poy+80,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "ON", "CONT", "OFF", "CANCEL",	dnON,	upON },
    { 45,	pox+xstart+(xspacing2*1),   ystart+(8*yspacing)+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "0", "=", "->", "",	dn03,	up03 },
    { 46,	pox+xstart+(xspacing2*2),  ystart+(8*yspacing)+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, ".", ",", "back", "",	dn02,	up02 },
    { 47, 	pox+xstart+(xspacing2*3),  ystart+(8*yspacing)+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "SPC", "pi", "rad", "",	dn01,	up01 },
    { 48,	pox+xstart+(xspacing2*4),  ystart+(8*yspacing)+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "+", "{}", ": :", "",	dn00,	up00 },
	
	{ 49,	pox+xstart,    ystart+(9*yspacing)+poy+90,	130, 20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "Game", "", "", "",	dnZelda,	upZelda },
    { 50,	pox+xstart,    poy,	    0,	0,  0,					NULL,	NULL, NULL, NULL,	NULL }
};


void pcalc_init()
{
	gui_initKeyboard(calc_buttons);
}

void pcalc_show()
{
	//clear_to_color(calc_bmp, color[C_PANEL_BACK]);
    button_draw_all(calc_buttons);
}


void pcalc_hide(void)
{
    //calc_bmp = NULL;
}

void pcalc_down(int mx, int my, int mb)
{
    button_mouse_down(calc_buttons, mx, my, mb);
}

void pcalc_up(int mx, int my, int mb)
{
    button_mouse_up(calc_buttons, mx, my, mb);
}

void pcalc_kb_down(SDL_Keycode sdl_event)
{
	printf("%d\n", SDLK_0);
	KBMapping * mapping = kb_sdl_mapping;
	while(mapping->SDL_event_id)
	{
		if(sdl_event == mapping->SDL_event_id) {
			mapping->down();
			break;
		}
		mapping ++;
	}
}

void pcalc_kb_up(SDL_Keycode sdl_event)
{
	KBMapping * mapping = kb_sdl_mapping;
	while(mapping->SDL_event_id)
	{
		if(sdl_event == mapping->SDL_event_id) {
			mapping->up(TRUE);
			break;
		}
		mapping ++;
	}
}
