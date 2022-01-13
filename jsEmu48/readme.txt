

Source : https://github.com/brizzly/jsEmu48

Demo : http://jadegame.com/jsemu48/hp48.html


SDL2

https://www.libsdl.org/projects/SDL_ttf/
https://www.libsdl.org/download-2.0.php
https://www.libsdl.org/projects/SDL_image/



Emscripten 

emcc -O3 bus.c color.c cpu.c disasm.c display.c emulator.c gui.c hdw.c  keyboard.c  main.c  opcodes.c pabout.c pcalc.c pdebug.c pfiles.c pmenu.c ports.c ram.c rom.c rpl.c timers.c -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file hpemu.rom --preload-file FreeSans.ttf --preload-file 48face5.png --preload-file zeldahp.dir -o hp48.js

