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

BIN = hpemu.bin
OBJS = bus.o color.o cpu.o disasm.o display.o emulator.o gui.o hdw.o keyboard.o main.o opcodes.o pabout.o pcalc.o pdebug.o pfiles.o pmenu.o ports.o ram.o rom.o rpl.o state.o timers.o

CC = gcc
UNAME_S := $(shell uname -s)

# Platform-specific SDL2 configuration
ifeq ($(UNAME_S),Darwin)
    # macOS with Homebrew
    SDL_PREFIX = $(shell brew --prefix sdl2)
    SDL_TTF_PREFIX = $(shell brew --prefix sdl2_ttf)
    SDL_IMAGE_PREFIX = $(shell brew --prefix sdl2_image)
    SDL_CFLAGS = -I$(SDL_PREFIX)/include -I$(SDL_PREFIX)/include/SDL2 \
                 -I$(SDL_TTF_PREFIX)/include -I$(SDL_TTF_PREFIX)/include/SDL2 \
                 -I$(SDL_IMAGE_PREFIX)/include -I$(SDL_IMAGE_PREFIX)/include/SDL2 \
                 -D_THREAD_SAFE
    SDL_LIBS = -L$(SDL_PREFIX)/lib -L$(SDL_TTF_PREFIX)/lib -L$(SDL_IMAGE_PREFIX)/lib \
               -lSDL2 -lSDL2_ttf -lSDL2_image -framework CoreFoundation
else
    # Linux - use pkg-config or sdl2-config
    SDL_CFLAGS = $(shell sdl2-config --cflags)
    SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image
endif

CFLAGS = -Wall -Werror -O3 -Wno-error=unused-function -Wno-error=unused-variable \
         -Wno-error=unused-but-set-variable -Wno-error=missing-braces \
         -Wno-error=incompatible-pointer-types -Wno-error=implicit-function-declaration \
         -Wno-error=misleading-indentation \
         $(SDL_CFLAGS)

all: $(BIN)

clean-all: clean
	-rm $(BIN)

clean:
	-rm $(OBJS)

$(BIN): $(OBJS)
	$(CC) -o $@ $+ $(SDL_LIBS)

%.o: hpemu/src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

bus.o: hpemu/src/types.h hpemu/src/rom.h hpemu/src/ram.h hpemu/src/ports.h hpemu/src/hdw.h hpemu/src/bus.h
color.o: hpemu/src/color.h
cpu.o: hpemu/src/types.h hpemu/src/emulator.h hpemu/src/bus.h hpemu/src/opcodes.h hpemu/src/cpu.h
cpu.o: hpemu/src/types.h hpemu/src/opcodes.h hpemu/src/disasm.h
display.o: hpemu/src/types.h hpemu/src/bus.h
emulator.o: hpemu/src/types.h hpemu/src/cpu.h hpemu/src/bus.h hpemu/src/timers.h hpemu/src/display.h hpemu/src/gui.h hpemu/src/pdebug.h hpemu/src/emulator.h
gui.o: hpemu/src/color.h hpemu/src/pmenu.h hpemu/src/pcalc.h hpemu/src/pdebug.h hpemu/src/pfiles.h hpemu/src/pabout.h hpemu/src/gui.h
hdw.o: hpemu/src/types.h hpemu/src/bus.h hpemu/src/ports.h hpemu/src/timers.h hpemu/src/display.h hpemu/src/hdw.h
keyboard.o: hpemu/src/types.h hpemu/src/cpu.h hpemu/src/keyboard.h
main.o: hpemu/src/types.h hpemu/src/emulator.h hpemu/src/gui.h hpemu/src/color.h
opcodes.o: hpemu/src/types.h hpemu/src/cpu.h hpemu/src/bus.h hpemu/src/keyboard.h hpemu/src/opcodes.h hpemu/src/opinline.h
pabout.o: hpemu/src/color.h hpemu/src/gui.h hpemu/src/pabout.h
pcalc.o: hpemu/src/color.h hpemu/src/display.h hpemu/src/keyboard.h hpemu/src/gui.h hpemu/src/pcalc.h
pdebug.o: hpemu/src/types.h hpemu/src/emulator.h hpemu/src/cpu.h hpemu/src/bus.h hpemu/src/disasm.h hpemu/src/color.h hpemu/src/gui.h hpemu/src/pdebug.h
pfiles.o: hpemu/src/color.h hpemu/src/gui.h hpemu/src/rpl.h hpemu/src/pfiles.h
pmenu.o: hpemu/src/emulator.h hpemu/src/color.h hpemu/src/gui.h hpemu/src/pmenu.h
ports.o: hpemu/src/types.h hpemu/src/bus.h hpemu/src/ports.h
ram.o: hpemu/src/types.h hpemu/src/bus.h hpemu/src/ram.h
rom.o: hpemu/src/types.h hpemu/src/bus.h hpemu/src/rom.h
rpl.o: hpemu/src/types.h hpemu/src/bus.h hpemu/src/opinline.h hpemu/src/rpl.h
timers.o: hpemu/src/types.h hpemu/src/cpu.h hpemu/src/timers.h


js:
	emcc -O3 hpemu/src/*.c \
	-Wno-error=incompatible-pointer-types -Wno-error=incompatible-function-pointer-types \
	-Wno-error=implicit-function-declaration -Wno-error=int-conversion \
	-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 \
	-s EXPORTED_RUNTIME_METHODS='["FS","callMain","ccall"]' -s FORCE_FILESYSTEM=1 \
	-s EXPORTED_FUNCTIONS='["_main","_hp_save_state","_hp_key_down","_hp_key_up","_hp_on_down","_hp_on_up"]' \
	-s ALLOW_MEMORY_GROWTH=1 \
	--preload-file hpemu/src/48face5.png \
	--preload-file assets \
	-o jsEmu48/hp48.html \
	-sWASM=0 --memory-init-file 1

js-clean:
	-rm jsEmu48/hp48.*

js-serve:
	ruby -run -e httpd ./jsEmu48/ -p 9999
