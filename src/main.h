#ifndef GAME_MAIN_H
#define GAME_MAIN_H
#define min(a,b)   (((a) < (b)) ? (a) : (b))
#define max(a,b)   (((a) > (b)) ? (a) : (b))
#include <SDL2/SDL.h>
#include <stdio.h>
#include "gfx.h"

int condition;
SDL_bool playerhit;
SDL_bool retry;
int bosshp,status;
Uint32 tag;
FILE *file;



void scanhitbox(int ability,pos charpos, pos bosspos);
void mainfunc(SDL_bool* screenrunning);
void boss(pos *bosspos,pos charpos);
void selector(int* dist, int* abl);
SDL_bool timer(int delay,Uint32 *tagp);


typedef struct node {
    int val;
    struct node* next;
} node;
int kills[5];

SDL_bool addtolists(node** lists);
void setratio(node** lists);
void freelist(node** lists);

#endif //GAME_MAIN_H

