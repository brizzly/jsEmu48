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

#include "types.h"
#include "cpu.h"
#include "timers.h"

#define TIMER1_EXTRA	0x1
#define TIMER1_INT	0x2
#define TIMER1_WAKE	0x4
#define TIMER1_SRQ	0x8

#define TIMER_RUN	0x1
#define TIMER2_INT	0x2
#define TIMER2_WAKE	0x4
#define TIMER2_SRQ	0x8

byte timer1_control;
byte timer2_control;
byte timer1_value;
dword timer2_value;

void timer1_update(void)
{
    if (timer2_control & TIMER_RUN)
	{
		timer1_value--;
		timer1_value &= 0xF;
   
		if (timer1_value & 0x8)
		{
			if (timer1_control & TIMER1_WAKE)
			{
				timer1_control |= TIMER1_SRQ;
				
				if (cpu.shutdown)
				{
					cpu.shutdown = FALSE;
					timer1_control &= ~TIMER1_WAKE;
				}
			}
			if (timer1_control & TIMER1_INT)
			{
				timer1_control |= TIMER1_SRQ;
				
				if (!cpu.shutdown) {
					cpu_interrupt();
				}
			}
		}
		else {
	    	timer1_control &= ~TIMER1_SRQ;
		}
    }
}

void timer2_update(void)
{
    if (timer2_control & TIMER_RUN) {
	timer2_value--;
	if (timer2_value & 0x80000000) {
	    if (timer2_control & TIMER2_WAKE) {
		timer2_control |= TIMER2_SRQ;
		if (cpu.shutdown) {
		    cpu.shutdown = 0;
		    timer2_control &= ~TIMER2_WAKE;
		}
	    }
	    if (timer2_control & TIMER2_INT) {
		timer2_control |= TIMER2_SRQ;
		if (!cpu.shutdown) {
		    cpu_interrupt();
		}
	    }
	} else {
	    timer2_control &= ~TIMER2_SRQ;
	}
    }
}
