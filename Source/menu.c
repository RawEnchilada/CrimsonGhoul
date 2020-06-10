#include "gfx.h"
#include "main.h"
#include "boss.h"
#include <debugmalloc.h>


void menu(SDL_bool* screenrunning){
    SDL_bool inmenu = SDL_TRUE,mlb = SDL_FALSE;
    SDL_bool showcontrols = SDL_FALSE;
    int buttons = 3,kode = 0;
    SDL_Event event;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    pos mouse;
    SDL_Rect btn2 = {257,230,126,27};
    SDL_Rect btn3 = {257,260,126,27};
    cheat = SDL_FALSE;

    //linked lists created in arrays of lists depending on ability
    node *lists[5];
    if(addtolists(lists)) {
        setratio(lists);
        freelist(lists);
    }
    else{
        for (int i = 0; i < 5; ++i) {
            rat[i] = 20;
        }
    }

    while(inmenu && !retry) {
        SDL_Rect btn = {257,200,126,27};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case (SDL_QUIT):
                    *screenrunning = SDL_FALSE;
                    inmenu = SDL_FALSE;
                    break;
                case(SDL_KEYDOWN):
                    if(keystates[SDL_SCANCODE_2] && kode == 0)kode = 1;
                    else if(keystates[SDL_SCANCODE_H] && kode == 1)kode = 2;
                    else if(keystates[SDL_SCANCODE_A] && kode == 2)kode = 3;
                    else if(keystates[SDL_SCANCODE_R] && kode == 3)kode = 4;
                    else if(keystates[SDL_SCANCODE_D] && kode == 4){
                        cheat = cheat ? SDL_FALSE:SDL_TRUE;
                        kode = 0;
                    }
                    else kode = 0;
                    break;
            }
            SDL_GetMouseState(&mouse.x, &mouse.y);

            if(over(mouse,btn))buttons = 0;
            else if(over(mouse,btn2))buttons = 1;
            else if(over(mouse,btn3))buttons = 2;
            else buttons = 3;
            if(keystates[SDL_SCANCODE_ESCAPE])showcontrols = SDL_FALSE;
            if(event.type == SDL_MOUSEBUTTONDOWN)mlb = SDL_TRUE;
            else if(event.type == SDL_MOUSEBUTTONUP)mlb = SDL_FALSE;
            if(mlb && buttons == 0){
                inmenu = SDL_FALSE;
            }
            else if((mlb && buttons == 1)|| showcontrols){
                showcontrols = SDL_TRUE;
            }
            else if(mlb && buttons == 2){
                inmenu = SDL_FALSE;
                *screenrunning = SDL_FALSE;
            }
        }
        rendermenu(buttons,btn,showcontrols);
    }
    retry = SDL_FALSE;
}
