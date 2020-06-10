#include "gfx.h"
#include "main.h"
#include "boss.h"
#include <math.h>
#include <debugmalloc.h>


SDL_bool hitscan(hitbox source, hitbox dest){
    if(!((min(dest.x1,dest.x2) > max(source.x1,source.x2)) ||
         (max(dest.x1,dest.x2) < min(source.x1,source.x2)) ||
         (min(dest.y1,dest.y2) > max(source.y1,source.y2)) ||
         (max(dest.y1,dest.y2) < min(source.y1,source.y2))))return SDL_TRUE;
    else return SDL_FALSE;
}

int distance(hitbox hb1,hitbox hb2){
    double val = hypot(((hb1.x1+hb1.x2)/2)-((hb2.x1+hb2.x2)/2),((hb1.y1+hb1.y2)/2)-((hb2.y1+hb2.y2)/2));
    if(playerhit)val = 1;
    return (int)val;
}

void scanhitbox(int ability,pos charpos,pos bosspos) {
    hitbox playerhb = {charpos.x, charpos.y,charpos.x+32,charpos.y+32};
    hitbox swordhb = {0,charpos.y+8,0,charpos.y+24};
    hitbox bosshb = {bosspos.x+75,bosspos.y+100,bosspos.x+225,bosspos.y+300};
    hitbox hammerhb = {0,bosspos.y+225,0,bosspos.y+300};
    hitbox stomphb = {bosspos.x+75,bosspos.y+275,bosspos.x+225,bosspos.y+300};
    hitbox areahb = {bosspos.x+90,bosspos.y+125,bosspos.x+210,bosspos.y+300};
    hitbox pokehb = {0,bosspos.y+200,0,bosspos.y+260};
    hitbox crackhb = {charpos.x,300,charpos.x+32,332};


    if(face){
        swordhb.x1 = charpos.x+16;
        swordhb.x2 = swordhb.x1+32;
    }
    else{
        swordhb.x1 = charpos.x-16;
        swordhb.x2 = swordhb.x1+32;
    }
    if(cstance == &charcrouch || cstance == &charroll1 || cstance == &charroll2 || cstance == &charroll3){
        playerhb.y1 = charpos.y+12;
    }

    if(bossface){
        hammerhb.x1 = bosspos.x+225;
        hammerhb.x2 = hammerhb.x1+50;
        pokehb.x1 = bosspos.x+200;
        pokehb.x2 = pokehb.x1+50;
    }
    else{
        hammerhb.x1 = bosspos.x+25;
        hammerhb.x2 = hammerhb.x1+50;
        pokehb.x1 = bosspos.x+50;
        pokehb.x2 = pokehb.x1+50;
    }
    switch(ability){
        case(5):
            if(hitscan(swordhb,bosshb))bosshp -= 2;
            break;
        case(0):
            if(hitscan(hammerhb,playerhb))playerhit = !cheat;
            fprintf(file,"smash\n%d\n",distance(hammerhb,playerhb));
            break;
        case(1):
            if(hitscan(stomphb,playerhb))playerhit = !cheat;
            fprintf(file,"stomp\n%d\n",distance(stomphb,playerhb));
            break;
        case(2):
            if(hitscan(crackhb,playerhb))playerhit = !cheat;
            fprintf(file,"crack\n%d\n",distance(crackhb,playerhb));
            break;
        case(3):
            if(hitscan(areahb,playerhb))playerhit = !cheat;
            fprintf(file,"area\n%d\n",distance(areahb,playerhb));
            break;
        case(4):
            if(hitscan(pokehb,playerhb))playerhit = !cheat;
            fprintf(file,"poke\n%d\n",distance(pokehb,playerhb));
            break;
    }
    if(playerhit){
        condition = 1;
    }
    else if(bosshp == 0){
        condition = 2;
    }
}





