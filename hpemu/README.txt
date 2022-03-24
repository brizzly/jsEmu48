    /
   /__  ___  ___  ____
  /  / /  / /__/ / / / /  /
 /  / /__/ /__  /   / /__/
     /
    /    version 0.9.0

Copyright 2002 Daniel Nilsson

This file is part of hpemu.

Hpemu is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Hpemu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with hpemu; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

The GNU General Public License is included in the file COPYING.txt


********************
*** INTRODUCTION ***
********************

Hpemu is an emulator for the HP48GX calculator.
Hpemu is intended for developers of software for the calculator but
other people may find it usefull too.
It is developed for the windows platform but should be easy to port to
other platforms.


****************
*** FEATURES ***
****************

Features of the HP48 that are implemented in this version:

 * Saturn CPU (most instructions)
 * Daisy-chain
 * ROM
 * RAM
 * Card ports 1 & 2 (untested)
 * Bank-switcher (untested)
 * Hardware registers (some of them)
 * Hardware CRC
 * Timer1 (not complete)
 * Timer2 (not complete)
 * Interrup system (not complete)
 * LCD (grayscale)
 * Keyboard (not complete)

Unimplemented features:

 * Indicators above LCD
 * UART
 * IR
 * Speaker
 * Things marked "not complete"

********************
*** INSTALLATION ***
********************

If you have the Windows binary distribution (hpemu-X.Y.Z-win32.zip):

1. Unzip the file in a new directory. You should get these files:

  bin\hpemu.exe   - The program itself
  bin\allegNN.dll - Allegro DLL needed to run hpemu
  README.txt      - This file
  COPYING.txt     - The GNU General Public License

2. Get a dump of the calculator's ROM and save it as hpemu.rom in the
   same directory as phemu.exe. (ROM revision R is known to work)

3. Execute hpemu.exe


If you have the source distribution (hpemu-X.Y.Z-src.zip):

1. Unzip the file in a new directory.
   You should get a bunch of .c and .h files in the src directory
   and some other files.

2. Make sure you have the Allegro library and headers properly
   installed (http://alleg.sourceforge.net/)

3a. A makefile for MinGW is included so if that is your compiler just
    execute make to build the program.

3b. If you use another compiler or platform you will need to get all
    files compiled and linked against the Allegro library. If you get
    hpemu to work on another platform please contact me and tell me if
    you had any problems.

4. Get a dump of the calculators ROM and save it as hpemu.rom in the
   same directory as the executable. (ROM revision R is known to work)

5. Execute the program.


****************************
*** COMMAND LINE OPTIONS ***
****************************

The command line options are:

 -f	Run program i fullscreen
 -w	Run program i a window


********************
*** CONTACT INFO ***
********************

Latest version of hpemu can allways be found on the hpemu homepage:
sourceforge.net/projects/hpemu

Contact the author:
 Daniel Nilsson <dannil@users.sourceforge.net>
