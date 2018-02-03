# jsEmu48
SDL Emscripten compatible Fork version of HPEMU by Daniel Nilsson

Demo : http://jadegame.com/jsemu48/hp48.html



Emscripten 

emcc -O3 *.c -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file hpemu.rom --preload-file FreeSans.ttf --preload-file 48face4.png --preload-file zeldahp.dir -o hp48.js



HP EMU 0.9
https://sourceforge.net/projects/hpemu/files/hpemu/


Visit my HP48 page : http://jadegame.com/games/hp48
