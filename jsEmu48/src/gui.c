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
#include <stdio.h>
#include "color.h"
#include "pmenu.h"
#include "pcalc.h"
#include "pdebug.h"
#include "pfiles.h"
#include "pabout.h"
#include "gui.h"

#ifdef __EMSCRIPTEN__
  #include "SDL.h"
  #include "SDL_image.h"
  #ifdef SDL_TTF
    #include "SDL_ttf.h"
  #endif
#else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
  #ifdef SDL_TTF
    #include <SDL2/SDL_ttf.h>
  #endif
#endif

extern SDL_Renderer * renderer;
extern SDL_Texture * faceplateTexture;
#ifdef SDL_TTF
extern TTF_Font * ArialFonte;
extern TTF_Font * ArialFonte2;
#endif

SDL_Surface * surfA[49];
SDL_Texture * textA[49];

SDL_Surface * surfB[49];
SDL_Texture * textB[49];

SDL_Surface * surfC[49];
SDL_Texture * textC[49];

SDL_Surface * surfD[49];
SDL_Texture * textD[49];



#define PANEL_FLAG_VISIBLE	0x01

typedef struct GuiPanel {
    int x, y;
    int w, h;
    int flags;
    //BITMAP *bmp;
    //void (*show)(BITMAP *bmp);
    void (*hide)(void);
    void (*mouse_down)(int mx, int my, int mb);
    void (*mouse_up)(int mx, int my, int mb);
} GuiPanel;

static GuiPanel panels[PANEL_COUNT] = {
	/*
    { 10,   10,	    620,    20,	    0,	NULL,	pmenu_show,	pmenu_hide,	pmenu_down,	pmenu_up },
    { 360,  40,	    270,    430,    0,	NULL,	pcalc_show,	pcalc_hide,	pcalc_down,	pcalc_up },
    { 10,   40,	    340,    430,    0,	NULL,	pdebug_show,	pdebug_hide,	pdebug_down,	pdebug_up },
    { 10,   40,	    340,    430,    0,	NULL,	pfiles_show,	pfiles_hide,	pfiles_down,	pfiles_up },
    { 10,   40,	    340,    430,    0,	NULL,	pabout_show,	pabout_hide,	pabout_down,	pabout_up },
	 */
};

void drawText(int index, int x, int y, int btn_w, int btn_h)
{
	SDL_Surface * letterSurface = surfA[index];
	SDL_Texture * letterTexture = textA[index];
	if(letterSurface != NULL && letterTexture != NULL) {
		int texW = letterSurface->w;
		int texH = letterSurface->h;
		SDL_Rect destRect = {x + (btn_w-texW)/2, y, texW, texH};
		SDL_RenderCopy(renderer, letterTexture, NULL, &destRect);
	}

	SDL_Surface * letterSurface2 = surfB[index];
	SDL_Texture * letterTexture2 = textB[index];
	if(letterSurface2 != NULL && letterTexture2 != NULL) {
		int texW = letterSurface2->w;
		int texH = letterSurface2->h;
		SDL_Rect destRect = {x + (btn_w-texW)/2, y, texW, texH};
		destRect.y -= 10;
		destRect.x -= 16;
		SDL_RenderCopy(renderer, letterTexture2, NULL, &destRect);
	}
	
	SDL_Surface * letterSurface3 = surfC[index];
	SDL_Texture * letterTexture3 = textC[index];
	if(letterSurface3 != NULL && letterTexture3 != NULL) {
		int texW = letterSurface3->w;
		int texH = letterSurface3->h;
		SDL_Rect destRect = {x + (btn_w-texW)/2, y, texW, texH};
		destRect.y -= 10;
		destRect.x += 16;
		SDL_RenderCopy(renderer, letterTexture3, NULL, &destRect);
	}
	
	SDL_Surface * letterSurface4 = surfD[index];
	SDL_Texture * letterTexture4 = textD[index];
	if(letterSurface4 != NULL && letterTexture4 != NULL) {
		int texW = letterSurface4->w;
		int texH = letterSurface4->h;
		SDL_Rect destRect = {x + (btn_w-texW)/2, y, texW, texH};
		destRect.y += 12;
		destRect.x += 22;
		SDL_RenderCopy(renderer, letterTexture4, NULL, &destRect);
	}
}

void gui_initKeyboard(Button * calcbuttons)
{
	printf("init texts\n");
#ifdef SDL_TTF
	if(ArialFonte == NULL) {
		printf("init texts error Font NULL\n");
		return;
	}
#endif
	SDL_Color couleurBlanche = {255, 255, 255};
	SDL_Color couleurGreen = {125, 215, 235};
	SDL_Color couleurPurple = {191, 192, 236};
	SDL_Color couleurYellow = {128, 108, 29};
	
#ifdef SDL_TTF
	int i=0;
	Button *buttons = calcbuttons;
	while(buttons->text)
	{
		SDL_Surface * s = NULL;
		SDL_Texture * t = NULL;
		if(buttons->text && strcmp(buttons->text, "") != 0)
		{
			s = TTF_RenderText_Blended (ArialFonte, buttons->text, couleurBlanche);
			if(s) {
				t = SDL_CreateTextureFromSurface( renderer, s );
			}
		}
		
		surfA[i] = s;
		textA[i] = t;

		i++;
		buttons++;
	}
	
	i=0;
	buttons = calcbuttons;
	while(buttons->textB)
	{
		SDL_Surface * s = NULL;
		SDL_Texture * t = NULL;
		if(buttons->textB && strcmp(buttons->textB, "") != 0)
		{
			s = TTF_RenderText_Blended (ArialFonte2, buttons->textB, couleurPurple);
			if(s) {
				t = SDL_CreateTextureFromSurface( renderer, s );
			}
		}
		surfB[i] = s;
		textB[i] = t;
		i++;
		buttons++;
	}
	
	i=0;
	buttons = calcbuttons;
	while(buttons->textC)
	{
		SDL_Surface * s = NULL;
		SDL_Texture * t = NULL;
		if(buttons->textC && strcmp(buttons->textC, "") != 0)
		{
			s = TTF_RenderText_Blended (ArialFonte2, buttons->textC, couleurGreen);
			if(s) {
				t = SDL_CreateTextureFromSurface( renderer, s );
			}
		}
		surfC[i] = s;
		textC[i] = t;
		i++;
		buttons++;
	}
	
	i=0;
	buttons = calcbuttons;
	while(buttons->textC)
	{
		SDL_Surface * s = NULL;
		SDL_Texture * t = NULL;
		if(buttons->textD && strcmp(buttons->textD, "") != 0)
		{
			s = TTF_RenderText_Blended (ArialFonte2, buttons->textD, couleurYellow);
			if(s) {
				t = SDL_CreateTextureFromSurface( renderer, s );
			}
		}
		surfD[i] = s;
		textD[i] = t;
		i++;
		buttons++;
	}
	
	
#endif
}

void gui_init(void)
{
}

void gui_exit(void)
{
	int i;
	
	for (i = 0; i < PANEL_COUNT; i++) {
		gui_hide_panel(i);
	}
}

static __inline int panel_at(int x, int y)
{
    int i;

    for (i = PANEL_COUNT; i >= 0; i--) {
	if (panels[i].flags & PANEL_FLAG_VISIBLE &&
	    x >= panels[i].x && x < panels[i].x+panels[i].w &&
	    y >= panels[i].y && y < panels[i].y+panels[i].h) {
	    break;
	}
    }
    return i;
}

void gui_update(void)
{
	/*
    static int down_panel = -1;
    static int down_mb = 0;
    int mx, my, mb;

    mx = mouse_x;
    my = mouse_y;
    mb = mouse_b;

    if (!down_mb && (mb & 1)) {
	down_panel = panel_at(mx, my);
	if (down_panel >= 0) {
	    down_mb = 1;
	    panels[down_panel].mouse_down(mx - panels[down_panel].x, my - panels[down_panel].y, down_mb);
	}
    } else if (!down_mb && (mb & 2)) {
	down_panel = panel_at(mx, my);
	if (down_panel >= 0) {
	    down_mb = 2;
	    panels[down_panel].mouse_down(mx - panels[down_panel].x, my - panels[down_panel].y, down_mb);
	}
    } else if (down_mb && !(mb & 3)) {
	panels[down_panel].mouse_up(mx - panels[down_panel].x, my - panels[down_panel].y, down_mb);
	down_mb = 0;
	down_panel = -1;
    }
	 */
}

void gui_show_panel(int i)
{
	/*
    if (!(panels[i].flags & PANEL_FLAG_VISIBLE)) {
	panels[i].flags |= PANEL_FLAG_VISIBLE;
	panels[i].bmp = create_sub_bitmap(screen, panels[i].x, panels[i].y, panels[i].w, panels[i].h);
	acquire_screen();
	scare_mouse();
	rect(screen, panels[i].x-1, panels[i].y-1, panels[i].x+panels[i].w, panels[i].y+panels[i].h, color[C_PANEL_BORDER]);
	panels[i].show(panels[i].bmp);
	unscare_mouse();
	release_screen();
    }*/
}

void gui_hide_panel(int i)
{
	/*
    if (panels[i].flags & PANEL_FLAG_VISIBLE) {
	panels[i].flags &= ~PANEL_FLAG_VISIBLE;
	panels[i].hide();
	destroy_bitmap(panels[i].bmp);
	panels[i].bmp = NULL;
    }*/
}


void button_draw(Button *b)
{
	SDL_Rect rectToDraw = {b->x*2, b->y*2, b->w*2, b->h*2};

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x33);
	SDL_RenderFillRect(renderer, &rectToDraw);

	drawText(b->index, b->x*2, 10 + b->y*2, b->w*2, b->h*2);

	int c;
	
	c = color[(b->flags&BUTTON_PUSHED) ? C_BUTTON_PUSHED : C_BUTTON_BACK];

	if(b->flags&BUTTON_PUSHED) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &rectToDraw);
	}
	else {
		// debug only
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(renderer, &rectToDraw);
	}

	
	
	/*
    int c;
   
    c = color[(b->flags&BUTTON_PUSHED) ? C_BUTTON_PUSHED : C_BUTTON_BACK];
    text_mode(c);
    acquire_bitmap(bmp);
    scare_mouse();
    rect(bmp, b->x-1, b->y-1, b->x+b->w, b->y+b->h, color[C_BUTTON_BORDER]);
    rectfill(bmp, b->x, b->y, b->x+b->w-1, b->y+b->h-1, c);
    c = color[(b->flags&BUTTON_DISABLED) ? C_BUTTON_DISABLED : C_BUTTON_TEXT];
    textout_centre(bmp, font, b->text, b->x+b->w/2, b->y+(b->h-text_height (font))/2, c);
    unscare_mouse();
    release_bitmap(bmp);
	 */
}

void button_draw_all(Button *buttons)
{
    while (buttons->text) {
		button_draw(buttons);
		buttons++;
    }
}


static __inline Button *find_button(Button *b, int x, int y)
{
    while (b->text) {
	//if (x >= b->x && x < b->x+b->w && y >= b->y && y < b->y+b->h) {
	if (x >= b->x*2 && x < b->x*2 + b->w*2 && y >= b->y*2 && y < b->y*2 + b->h*2) {
	    return b;
	}
	b++;
    }
    return NULL;
}


int button_mouse_down(Button *buttons, int mx, int my, int mb)
{
    Button *b = find_button(buttons, mx, my);
    if (!b) {
		return 0;
    }
	printf("Press %s\n", b->text);
	
    if (!(b->flags & BUTTON_DISABLED))
	{
		if ((mb == 2 && (b->flags & BUTTON_B2TOGGLE)) ||
			(mb == 1 && (b->flags & BUTTON_B1TOGGLE))) {
			
			if (b->flags & BUTTON_PUSHED) {
				b->flags &= ~BUTTON_PUSHED;
			//button_draw(bmp, b);
				if (b->up) b->up(TRUE);
			}
			else {
				b->flags |= BUTTON_PUSHED;
				//button_draw(bmp, b);
				if (b->down) b->down();
			}
		} else if (mb == 1 && !(b->flags & BUTTON_PUSHED)) {
			b->flags |= BUTTON_PUSHED;
			//button_draw(bmp, b);
			if (b->down) b->down();
		}
    }
    return 1;
}

int button_mouse_up(/*BITMAP *bmp,*/ Button *buttons, int mx, int my, int mb)
{
    Button *b = find_button(buttons, mx, my);
    int ret = (b != NULL);
	if(b) {
		printf("Release %s\n", b->text);
	}

    if (b && !(b->flags & BUTTON_DISABLED)) {
	if (mb == 1 && (b->flags & BUTTON_PUSHED) && !(b->flags & BUTTON_B1TOGGLE)) {
	    b->flags &= ~BUTTON_PUSHED;
	    //button_draw(bmp, b);
	    if (b->up) b->up(TRUE);
	}
    }
    if (mb == 1) {
	for (b = buttons; b->text; b++) {
	    if ((b->flags & (BUTTON_B1RELEASE|BUTTON_PUSHED)) == (BUTTON_B1RELEASE|BUTTON_PUSHED)) {
		b->flags &= ~BUTTON_PUSHED;
		//button_draw(bmp, b);
		if (b->up) b->up(FALSE);
		ret = 1;
	    }
	}
    }
    return ret;
}
