

Source : https://github.com/brizzly/jsEmu48

Demo : http://jadegame.com/jsemu48/hp48.html



Emscripten 


emcc -O3 *.c -s USE_SDL=2 -s USE_SDL_TTF=2 -s ASSERTIONS=2 -Werror --preload-file hpemu.rom --preload-file FreeSans.ttf -s SAFE_HEAP=1 -o hp48.html


Todo

Speed things up !



