#include "main.h"
#include "gfx.h"
#include <debugmalloc.h>
#define SDL_MAIN_HANDLED

int main(int args, char* argv[]){
    init();
    setbuf(stdout, 0);
    static SDL_bool screenrunning = SDL_TRUE;
    while(screenrunning){
        menu(&screenrunning);
        if(screenrunning)mainfunc(&screenrunning);
    }
    quit(&screenrunning);
    exit(0);
}

