#ifndef GAME_GFX_H
#define GAME_GFX_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct pos{
    int x;
    int y;
}pos;

void quit(SDL_bool* screenrunning);
void render(pos charpos, pos bosspos);
void rendermenu(int press,SDL_Rect btn,SDL_bool showcontrols);
void endgame(SDL_bool *inloop,SDL_Event event,SDL_Rect btn1,SDL_Rect btn2,SDL_bool* screenrunning);
void init();
void menu(SDL_bool* screenrunning);

typedef struct hitbox{
    int x1;
    int y1;
    int x2;
    int y2;
}hitbox;

static struct screen{
    SDL_Window* window;
    SDL_Renderer* renderer;
    int w;
    int h;
}screen;

SDL_bool face;
SDL_bool bossface;

SDL_Texture* wall;
SDL_Texture* darken;
SDL_Texture* hpframe;
SDL_Texture* hpbar;

SDL_Texture* bossstand;
SDL_Texture* bosssmash;
SDL_Texture* bosspresmash;
SDL_Texture* bossprestomp;
SDL_Texture* bossstomp;
SDL_Texture* rock;
SDL_Texture* bosscrack;
SDL_Texture* bossprecrack;
SDL_Texture* bossprearea;
SDL_Texture* bossarea;
SDL_Texture* bossprepoke;
SDL_Texture* bosspoke;
SDL_Texture* bossstep1;
SDL_Texture* bossstep2;
SDL_Texture* bossstep3;
SDL_Texture* bossstep4;

SDL_Texture* charstand;
SDL_Texture* charrun1;
SDL_Texture* charrun2;
SDL_Texture* charjump;
SDL_Texture* charfall;
SDL_Texture* charcrouch;
SDL_Texture* charroll1;
SDL_Texture* charroll2;
SDL_Texture* charroll3;
SDL_Texture* charattack;

SDL_Texture* *cstance;
SDL_Texture* *bstance;



SDL_bool over(pos temp, SDL_Rect dest);


#endif //GAME_GFX_H
