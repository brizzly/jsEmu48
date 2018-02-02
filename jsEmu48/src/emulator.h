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

#ifndef __EMULATOR_H
#define __EMULATOR_H

#include "types.h"

enum EmulatorStates {
    EMULATOR_STOP,
    EMULATOR_STEP,
    EMULATOR_RUN
};

void true_speed_proc(void);

extern volatile boolean please_exit;
extern dword emulator_speed;

void emulator_init(void);
void emulator_exit(void);
boolean emulator_run(void);

void emulator_set_state(int state);
int emulator_get_state(void);

#endif
