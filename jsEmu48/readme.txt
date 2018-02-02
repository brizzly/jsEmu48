

Emscripten 


emcc *.c -O0 -s USE_SDL=2 -s USE_SDL_TTF=2 -s ASSERTIONS=2 -Werror --preload-file hpemu.rom --preload-file FreeSans.ttf -s SAFE_HEAP=1 -o hp48.html


