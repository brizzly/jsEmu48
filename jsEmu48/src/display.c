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

#include "SDL.h"
#include "SDL_image.h"
#include <stdlib.h> 
#include "types.h"
#include "bus.h"
#include "pcalc.h"

#define LCD_X	0 //76
#define LCD_Y	110 //150

address menu_base;
address display_base;
address display_line_offset;
byte display_line_count;
byte display_height;
byte display_offset;
boolean display_enable;

byte lcdScreen[131*64];
byte prev_lcdScreen[131*64];
byte prev2_lcdScreen[131*64];
//byte prev3_lcdScreen[131*64];
byte lcdScreenGS[131*64];

static address cur_adr;
static boolean in_menu;
static byte off_cur_line;
static byte off_line;
static int off_cnt;
static boolean shouldClear = TRUE;
static boolean shouldRender = FALSE;
static int screen_draw_count = 0;
static boolean drawGS = FALSE;

extern SDL_Renderer * renderer;
extern SDL_Window * window;
extern SDL_Texture * texTarget;
extern SDL_Texture * tex2Target;
extern SDL_Texture * faceplateTexture;


void clearLCD()
{
//	SDL_SetRenderDrawColor(renderer, 0x44, 0x44, 0x66, 0xFF);
//	SDL_SetRenderDrawColor(renderer, 119, 172, 130, 0xFF); // vert clair
	SDL_SetRenderDrawColor(renderer, 48, 68, 90, 0xFF); // bleu foncé
	SDL_RenderClear(renderer);
	
	if(faceplateTexture) {
		//SDL_Rect r3 = {-10,0,534,1100};
		SDL_Rect r3 = {8,0,504,1124};
		SDL_RenderCopy(renderer, faceplateTexture, NULL, &r3);
	}
	//SDL_SetRenderTarget(renderer, texTarget);
}

void endLCD()
{
	//Reset render target
//	SDL_SetRenderTarget( renderer, NULL );
	
	
	//Show rendered to texture
	//gTargetTexture.render( 0, 0, NULL, angle, &screenCenter );
	SDL_Rect r1 = {0,0,131,64};
//	SDL_Rect r2 = {LCD_X,LCD_Y,262,128};
	SDL_Rect r2 = {LCD_X,LCD_Y,524,256};
//	SDL_Rect r2 = {LCD_X,LCD_Y,436,214};
//	SDL_Rect r2 = {LCD_X,LCD_Y,393,192};
	SDL_RenderCopyEx(renderer, texTarget, &r1, &r2, 0, NULL, SDL_FLIP_NONE);
	

	pcalc_show();
}


void renderLCD()
{
	//SDL_RenderPresent( renderer );
}


static void fade_lcd_line(int y)
{
	/*
    byte *lcd_line0 = (byte *)lcd->line[y*2];
    byte *lcd_line1 = (byte *)lcd->line[y*2+1];
    int x = 0;

    while (x < 131) {
	lcd_line0[x*2] = lcd_line0[x*2+1] = lcd_line1[x*2] = lcd_line1[x*2+1] = (lcd_line0[x*2] >> 1);
	x++;
    }
	*/
}

static address draw_lcd_line(address adr, int y)
{
//	printf("draw_lcd_line %d ", y);

    int x = 0;
    int bit = 0;
    byte data = 0;	//Initialized to remove warning
    byte *ptr;
    //byte *lcd_line0 = (byte *)lcd->line[y*2];
    //byte *lcd_line1 = (byte *)lcd->line[y*2+1];

    ptr = bus_fast_peek(NULL, adr, NULL);

    // Horisontal pixel offset
    if (!in_menu)
	{
		if (display_offset > 3) {
			ptr++;
		}
		
		data = *ptr++;
		data >>= display_offset & 3;
		bit = 4 - (display_offset & 3);
	}
	

	
	
	while (x < 131)
	{
		if (bit == 0) {
			data = *ptr++;
			bit = 4;
		}
		

		byte pixel = ((data & 1) << 6); // (lcd_line0[x*2] >> 1) | ((data & 1) << 6);
		if(pixel != '\0') {
			pixel = '\3';
			//printf("%c ", pixel);
		}
		
		byte pixelGS = lcdScreenGS[x+y*131];
		
		
		//prev3_lcdScreen[x+y*131] = prev2_lcdScreen[x+y*131];
		prev2_lcdScreen[x+y*131] = prev_lcdScreen[x+y*131];
		prev_lcdScreen[x+y*131] = lcdScreen[x+y*131];
		lcdScreen[x+y*131] = pixel;

		byte prev_pixel = prev_lcdScreen[x+y*131];
		byte prev2_pixel = prev2_lcdScreen[x+y*131];
		//byte prev3_pixel = prev3_lcdScreen[x+y*131];

		
		if(drawGS == TRUE)
		{
			if(prev2_pixel == '\0' && prev_pixel == '\0' && pixel == '\0')
			{
				pixelGS = '\0';
			}
			
			if(prev2_pixel == '\3' && prev_pixel == '\3' && pixel == '\3')
			{
				pixelGS = '\3';
			}
			
			if(prev2_pixel == '\0' && prev_pixel == '\3' && pixel == '\3')
			{
				pixelGS = '\2';
			}

			if(prev2_pixel == '\3' && prev_pixel == '\0' && pixel == '\0')
			{
				pixelGS = '\1';
			}
			
			
			if(prev2_pixel == '\3' && prev_pixel == '\3' && pixel == '\0')
			{
				pixelGS = '\2';
			}
			
			if(prev2_pixel == '\3' && prev_pixel == '\0' && pixel == '\3')
			{
				pixelGS = '\2';
			}
			
			if(prev2_pixel == '\0' && prev_pixel == '\0' && pixel == '\3')
			{
				pixelGS = '\1';
			}

			if(prev2_pixel == '\0' && prev_pixel == '\3' && pixel == '\0')
			{
				pixelGS = '\1';
			}
			
			
			lcdScreenGS[x+y*131] = pixelGS;
		}
		
		//lcd_line0[x*2] = lcd_line0[x*2+1] = lcd_line1[x*2] = lcd_line1[x*2+1] = (lcd_line0[x*2] >> 1) | ((data & 1) << 6);
		
		data >>= 1;
		bit--;
		x++;
    }
	
    return (adr + 0x22 + (!in_menu && (display_offset&4)?2:0)) & 0xFFFFF;
}

void display_init(void)
{
    //lcd = create_bitmap_ex(8, 131*2, 64*2);
    //clear_to_color(lcd, 0);
}

void display_exit(void)
{
    //destroy_bitmap (lcd);
    //lcd = NULL;
}

void display_show()
{
	SDL_SetRenderDrawColor(renderer, 48, 68, 90, 0xFF); // bleu foncé
	SDL_RenderClear(renderer);
		
	if(faceplateTexture) {
		//SDL_Rect r3 = {-10,0,534,1100};
		SDL_Rect r3 = {8,0,504,1124};
		SDL_RenderCopy(renderer, faceplateTexture, NULL, &r3);
	}
	

	if(shouldRender == TRUE)
	{
		shouldRender = FALSE;
		
		int pitch, w, h;
		Uint32 * pixels;
		int access;
		Uint32 format;
		
		if ( SDL_QueryTexture(texTarget, &format, &access, &w, &h) != 0)
		{
			printf("error\n");
		}
		
		if ( SDL_LockTexture(texTarget, NULL, (void**)&pixels, &pitch) != 0)
		{
			printf("SDL_LockTexture: %s.\n", SDL_GetError());
		}
		
		
		SDL_PixelFormat * pixelFormat = SDL_AllocFormat( format );
		
		
		// do stuff
		for(int y=0 ; y<64 ; y++)
		{
			for(int x=0 ; x<131; x++)
			{
				int R = 0;
				int G = 0;
				int B = 0;
				
				//byte hp48pixel = lcdScreen[x+y*131];
				byte hp48pixel = lcdScreenGS[x+y*131];
			//	printf("%d ", hp48pixel);
				
				if(hp48pixel == '\0')
				{
					R = 119;
					G = 153;
					B = 136;
				}
				else if(hp48pixel == '\1')
				{
					R = 71; //200;
					G = 134; //20;
					B = 145; //20;
				}
				else if(hp48pixel == '\2')
				{
					R = 13;//20;
					G = 108;//200;
					B = 111;//20;
				}
				else if(hp48pixel == '\3')
				{
					R = 37;
					G = 61;
					B = 84;
				}
				
				
				// Now you want to format the color to a correct format that SDL can use.
				// Basically we convert our RGB color to a hex-like BGR color.
				
				Uint32 color = SDL_MapRGB(pixelFormat, R, G, B);
				
				// Before setting the color, we need to know where we have to place it.
				Uint32 pixelPosition = y * (pitch / sizeof(unsigned int)) + x;

				pixels[pixelPosition] = color;
			}
		}
		
		SDL_UnlockTexture(texTarget);
			
	}
	
	//Show rendered to texture
	SDL_Rect r1 = {0,0,131,64};
	SDL_Rect r2 = {LCD_X,LCD_Y,524,256};
	SDL_RenderCopyEx(renderer, texTarget, &r1, &r2, 0, NULL, SDL_FLIP_NONE);

	pcalc_show();
	
	SDL_RenderPresent(renderer);

}

void display_update(void)
{
    if (!display_enable && !off_cnt) {		/* Turn off display */
		off_cnt = 1;
		off_cur_line = off_line = display_line_count;
		display_line_count = 0;
    }
	else if (display_enable && off_cnt) {	/* Turn on display */
		off_cnt = 0;
		display_line_count = 0;
		in_menu = 0;
		cur_adr = display_base;
    }
	
    if (!off_cnt) {	/* Display is on */
		
		cur_adr = draw_lcd_line(cur_adr, display_line_count);

	//	acquire_screen();
	//	scare_mouse_area(LCD_X, LCD_Y+display_line_count*2, 131*2, 2);
	//	blit(lcd, screen, 0, display_line_count*2, LCD_X, LCD_Y+display_line_count*2, 131*2, 2);
	//	unscare_mouse();
	//	release_screen();

		if (!in_menu) {
			cur_adr += display_line_offset;
		}
		if (display_line_count == display_height) {
			in_menu = 1;
			cur_adr = menu_base;
		}
		
		
		display_line_count++;
		
		if (display_line_count == 64)
		{
			display_line_count = 0;
			in_menu = 0;
			cur_adr = display_base;
			
			shouldRender = TRUE;
			
			
			screen_draw_count ++;
			if(screen_draw_count == 3) {
				
				screen_draw_count = 0;
			}
		}
		
		
		if(screen_draw_count == 0)
		{
			drawGS = TRUE;
		}
		else
		{
			drawGS = FALSE;
		}
		
    }
	else if (off_cnt <= 7) {	/* Display is off and still fading */
		
		off_cnt = 8; // julien
		
		/*
		fade_lcd_line(off_cur_line);

	//	acquire_screen();
	//	scare_mouse_area(LCD_X, LCD_Y+off_cur_line*2, 131*2, 2);
	//	blit(lcd, screen, 0, off_cur_line*2, LCD_X, LCD_Y+off_cur_line*2, 131*2, 2);
	//	unscare_mouse();
	//	release_screen();

		off_cur_line++;
		if (off_cur_line == 64) {
			off_cur_line = 0;
		}
		if (off_cur_line == off_line) {
			off_cnt++;
		}*/
    }
	
	if(shouldRender == TRUE)
	{
		//shouldRender = FALSE;
		//shouldClear = TRUE;
		//endLCD();
	}
}
