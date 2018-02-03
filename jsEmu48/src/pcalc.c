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



const int pox = 25;
const int poy = 55;
const int pow1 = 34;
const int poh1 = 18;
const int poh2 = 22;
const int pow2 = 38;

static Button calc_buttons[] = {
	
    { 0, 	5+pox,    160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE,	"",	dn14,	up14 },
    { 1,	49+pox-8,   160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "",	dn84,	up84 },
    { 2,	93+pox-16,   160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "",	dn83,	up83 },
    { 3,	137+pox-24,  160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "",	dn82,	up82 },
    { 4,	181+pox-32,  160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "",	dn81,	up81 },
    { 5,	225+pox-40,  160+poy,    pow1,	poh1, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "",	dn80,	up80 },
	
	{ 6,	5+pox,    184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "MTH",	dn24,	up24 },
    { 7,	49+pox-8,   184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "PRG",	dn74,	up74 },
    { 8,	93+pox-16,   184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "CST",	dn73,	up73 },
    { 9,	137+pox-24,  184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "VAR",	dn72,	up72 },
    { 10,	181+pox-32,  184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "^",	dn71,	up71 },
    { 11,	225+pox-40,  184+poy+10,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "NXT",	dn70,	up70 },
	
	{ 12,	5+pox,    208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "'",	dn04,	up04 },
    { 13,	49+pox-8,   208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "STO",	dn64,	up64 },
    { 14, 	93+pox-16,   208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "EVAL",	dn63,	up63 },
    { 15,	137+pox-24,  208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "<",	dn62,	up62 },
    { 16,	181+pox-32,  208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "v",	dn61,	up61 },
    { 17,	225+pox-40,  208+poy+20,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, ">",	dn60,	up60 },
	
	{ 18,	5+pox,    232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "SIN",	dn34,	up34 },
    { 19,	49+pox-8,   232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "COS",	dn54,	up54 },
    { 20,	93+pox-16,   232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "TAN",	dn53,	up53 },
    { 21,	137+pox-24,  232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "/x",	dn52,	up52 },
    { 22,	181+pox-32,  232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "yx",	dn51,	up51 },
    { 23,	225+pox-40,  232+poy+30,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "1/x",	dn50,	up50 },
	
	{ 24,	5+pox,    256+poy+40,    70,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "ENTER",dn44,	up44 },
    { 25,	85+pox-8,   256+poy+40,    pow1,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "+/-",	dn43,	up43 },
    { 26,	129+pox-16,  256+poy+40,    pow1,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "EEX",	dn42,	up42 },
    { 27,	181+pox-32,  256+poy+40,    pow1,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "DEL",	dn41,	up41 },
    { 28,	225+pox-40,  256+poy+40,    pow1,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "<-",	dn40,	up40 },
	
	{ 29,	5+pox,    280+poy+50,    pow1,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "a",	dn35,	up35 },
    { 30,	49+pox,   280+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "7",	dn33,	up33 },
    { 31,	104+pox-12,  280+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "8",	dn32,	up32 },
    { 32,	159+pox-24,  280+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "9",	dn31,	up31 },
    { 33,	214+pox-36,  280+poy+50,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "/",	dn30,	up30 },
	
	{ 34,	5+pox,    304+poy+60,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "<-",	dn25,	up25 },
    { 35,	49+pox,   304+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "4",	dn23,	up23 },
    { 36,	104+pox-12,  304+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "5",	dn22,	up22 },
    { 37,	159+pox-24,  304+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "6",	dn21,	up21 },
    { 38,	214+pox-36,  304+poy+60,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "X",	dn20,	up20 },
	
	{ 39,	5+pox,    328+poy+70,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "->",	dn15,	up15 },
    { 40,	49+pox,   328+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "1",	dn13,	up13 },
    { 41,	104+pox-12,  328+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "2",	dn12,	up12 },
    { 42,	159+pox-24,  328+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "3",	dn11,	up11 },
    { 43,	214+pox-36,  328+poy+70,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "-",	dn10,	up10 },
	
	{ 44,	5+pox,    352+poy+80,    pow1,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "ON",	dnON,	upON },
    { 45,	49+pox,   352+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "0",	dn03,	up03 },
    { 46,	104+pox-12,  352+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, ".",	dn02,	up02 },
    { 47, 	159+pox-24,  352+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "SPC",	dn01,	up01 },
    { 48,	214+pox-36,  352+poy+80,    pow2,	poh2, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "+",	dn00,	up00 },
	
	{ 49,	20,  530,    90,	20, BUTTON_B1RELEASE | BUTTON_B2TOGGLE, "Zelda",	dnZelda,	upZelda },
    { 50,	0+pox,    0+poy,	    0,	0,  0,					NULL,	NULL,	NULL }
};


void pcalc_init()
{
	gui_initKeyboard(calc_buttons);
}

void pcalc_show(/*BITMAP *bmp*/)
{
	//calc_bmp = bmp;

   // clear_to_color(calc_bmp, color[C_PANEL_BACK]);
    button_draw_all(/*calc_bmp,*/ calc_buttons);
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
