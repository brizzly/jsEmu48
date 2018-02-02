# jsEmu48
SDL Emscripten compatible Fork version of HPEMU by Daniel Nilsson

Source : https://github.com/brizzly/jsEmu48

Demo : http://jadegame.com/jsemu48/hp48.html



Emscripten 

emcc -O3 *.c -s USE_SDL=2 -s USE_SDL_TTF=2 -s ASSERTIONS=2 -Werror --preload-file hpemu.rom --preload-file FreeSans.ttf -s SAFE_HEAP=1 -o hp48.html



HP EMU 0.9

https://sourceforge.net/projects/hpemu/files/hpemu/



Todo

Speed things up !





