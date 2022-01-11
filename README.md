# jsEmu48
SDL Emscripten compatible Fork version of HPEMU by Daniel Nilsson

Demo : http://jadegame.com/jsemu48/


Install SDL2 + TTF + IMAGE

https://www.libsdl.org/projects/SDL_ttf/
https://www.libsdl.org/download-2.0.php
https://www.libsdl.org/projects/SDL_image/

Install git clone Emscripten

./emsdk install latest
./emsdk activate latest
./emsdk_env.bat


cd /src

emcc -O3 bus.c color.c cpu.c disasm.c display.c emulator.c gui.c hdw.c  keyboard.c  main.c  opcodes.c pabout.c pcalc.c pdebug.c pfiles.c pmenu.c ports.c ram.c rom.c rpl.c timers.c -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file hpemu.rom --preload-file FreeSans.ttf --preload-file 48face5.png --preload-file zeldahp.dir -o hp48.js



CREDITS

HP EMU 0.9
https://sourceforge.net/projects/hpemu/files/hpemu/


Visit my HP48 page : http://jadegame.com/games/hp48
