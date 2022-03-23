#     /
#    /__  ___  ___  ____
#   /  / /  / /__/ / / / /  /
#  /  / /__/ /__  /   / /__/
#      /
#     /    version 0.9.0
#
# Copyright 2002 Daniel Nilsson
#
# This file is part of hpemu.
#
# Hpemu is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Hpemu is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with hpemu; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

BIN = hpemu
OBJS = bus.o color.o cpu.o disasm.o display.o emulator.o gui.o hdw.o keyboard.o main.o opcodes.o pabout.o pcalc.o pdebug.o pfiles.o pmenu.o ports.o ram.o rom.o rpl.o timers.o

CC = gcc
CFLAGS = -Wall -Werror -O3 -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wno-error=missing-braces -Wno-error=incompatible-pointer-types

all: $(BIN) assets

clean-all: clean
	-rm $(BIN)
	-rm hpemu.rom FreeSans.ttf

clean:
	-rm $(OBJS)

assets: FreeSans.ttf hpemu.rom

$(BIN): $(OBJS)
	$(CC) -o $@ $+ -lSDL2 -lSDL2_ttf

FreeSans.ttf: jsEmu48/src/FreeSans.ttf
	cp $+ .

hpemu.rom: jsEmu48/src/hpemu.rom
	cp $+ .

%.o: jsEmu48/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

bus.o: jsEmu48/src/types.h jsEmu48/src/rom.h jsEmu48/src/ram.h jsEmu48/src/ports.h jsEmu48/src/hdw.h jsEmu48/src/bus.h
color.o: jsEmu48/src/color.h
cpu.o: jsEmu48/src/types.h jsEmu48/src/emulator.h jsEmu48/src/bus.h jsEmu48/src/opcodes.h jsEmu48/src/cpu.h
cpu.o: jsEmu48/src/types.h jsEmu48/src/opcodes.h jsEmu48/src/disasm.h
display.o: jsEmu48/src/types.h jsEmu48/src/bus.h
emulator.o: jsEmu48/src/types.h jsEmu48/src/cpu.h jsEmu48/src/bus.h jsEmu48/src/timers.h jsEmu48/src/display.h jsEmu48/src/gui.h jsEmu48/src/pdebug.h jsEmu48/src/emulator.h
gui.o: jsEmu48/src/color.h jsEmu48/src/pmenu.h jsEmu48/src/pcalc.h jsEmu48/src/pdebug.h jsEmu48/src/pfiles.h jsEmu48/src/pabout.h jsEmu48/src/gui.h
hdw.o: jsEmu48/src/types.h jsEmu48/src/bus.h jsEmu48/src/ports.h jsEmu48/src/timers.h jsEmu48/src/display.h jsEmu48/src/hdw.h
keyboard.o: jsEmu48/src/types.h jsEmu48/src/cpu.h jsEmu48/src/keyboard.h
main.o: jsEmu48/src/types.h jsEmu48/src/emulator.h jsEmu48/src/gui.h jsEmu48/src/color.h
opcodes.o: jsEmu48/src/types.h jsEmu48/src/cpu.h jsEmu48/src/bus.h jsEmu48/src/keyboard.h jsEmu48/src/opcodes.h jsEmu48/src/opinline.h
pabout.o: jsEmu48/src/color.h jsEmu48/src/gui.h jsEmu48/src/pabout.h
pcalc.o: jsEmu48/src/color.h jsEmu48/src/display.h jsEmu48/src/keyboard.h jsEmu48/src/gui.h jsEmu48/src/pcalc.h
pdebug.o: jsEmu48/src/types.h jsEmu48/src/emulator.h jsEmu48/src/cpu.h jsEmu48/src/bus.h jsEmu48/src/disasm.h jsEmu48/src/color.h jsEmu48/src/gui.h jsEmu48/src/pdebug.h
pfiles.o: jsEmu48/src/color.h jsEmu48/src/gui.h jsEmu48/src/rpl.h jsEmu48/src/pfiles.h
pmenu.o: jsEmu48/src/emulator.h jsEmu48/src/color.h jsEmu48/src/gui.h jsEmu48/src/pmenu.h
ports.o: jsEmu48/src/types.h jsEmu48/src/bus.h jsEmu48/src/ports.h
ram.o: jsEmu48/src/types.h jsEmu48/src/bus.h jsEmu48/src/ram.h
rom.o: jsEmu48/src/types.h jsEmu48/src/bus.h jsEmu48/src/rom.h
rpl.o: jsEmu48/src/types.h jsEmu48/src/bus.h jsEmu48/src/opinline.h jsEmu48/src/rpl.h
timers.o: jsEmu48/src/types.h jsEmu48/src/cpu.h jsEmu48/src/timers.h
