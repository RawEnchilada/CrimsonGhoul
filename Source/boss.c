#include "gfx.h"
#include "main.h"
#include <debugmalloc.h>


void smash(pos charpos,pos bosspos);
void stomp(pos charpos,pos bosspos);
void crack(pos charpos,pos bosspos);
void area(pos charpos,pos bosspos);
void poke(pos charpos,pos bosspos);
void walk(pos *bosspos);

void (*ability[5])(pos charpos,pos bosspos) = {smash,stomp,crack,area,poke};

Uint32 btag = -1;
Uint32 mtag = -1;
int dist = 0;
int abl = 0;
int s = 0;


int checkdist(int n,pos charpos,pos bosspos){
    if(abs((bosspos.x+150)-(charpos.x+16)) > n && ((bosspos.x+150)-(charpos.x+16)) > 0){
        return -1;
    }
    else if(abs((bosspos.x+150)-(charpos.x+16)) > n && ((bosspos.x+150)-(charpos.x+16)) < 0){
        return 1;
    }
    else if(abs((bosspos.x+150)-(charpos.x+16)) <= n){
        return 0;
    }
}

void boss(pos *bosspos,pos charpos){
    walk(bosspos);
    if(status == 0){
        selector(&dist, &abl);
        status = 1;
    }
    else if(status == 1) {
        switch (checkdist(dist,charpos,*bosspos)) {
            case (-1):
                bossface = SDL_FALSE;
                s = -5;
                break;
            case (1):
                bossface = SDL_TRUE;
                s = 5;
                break;
            case (0):
                s = 0;
                bstance = &bossstand;
                status = 2;
                break;
        }
    }
    else if(status == 2){
        (*ability[abl])(charpos,*bosspos);
    }
}

void walk(pos *bosspos){
    if(s != 0){
        if(timer(250,&mtag)) {
            bosspos->x += s;
            if (bstance == &bossstand || bstance == &bossstep4){
                bstance = &bossstep1;
            }
            else if(bstance == &bossstep1){
                bstance = &bossstep2;
            }
            else if(bstance == &bossstep2){
                bstance = &bossstep3;
            }
            else if(bstance == &bossstep3){
                bstance = &bossstep4;
            }

        }
    }
    else if(bstance == &bossstep1 ||bstance == &bossstep2||bstance == &bossstep3 ||bstance == &bossstep4){
        bstance = &bossstand;
    }
}

void smash(pos charpos,pos bosspos){
    if(bstance == &bossstand && timer(500,&btag)){
        bstance = &bosspresmash;
    }
    else if(bstance == &bosspresmash && timer(500, &btag)) {
        bstance = &bosssmash;
        scanhitbox(0,charpos,bosspos);
    }
    else if(bstance == &bosssmash && timer(300,&btag)){
        bstance = &bossstand;
        status = 0;
    }
}

void stomp(pos charpos,pos bosspos){
    if(bstance == &bossstand && timer(500,&btag)){
        bstance = &bossprestomp;
    }
    else if(bstance == &bossprestomp && timer(500, &btag)){
        bstance = &bossstomp;
        scanhitbox(1,charpos,bosspos);
    }
    else if(bstance == &bossstomp && timer(250,&btag)){
        bstance = &bossstand;
        status = 0;
    }
}

void crack(pos charpos,pos bosspos){
    if(bstance == &bossstand && timer(750,&btag)){
        bstance = &bossprecrack;
    }
    else if(bstance == &bossprecrack && timer(750,&btag)){
        bstance = &bosscrack;        
        scanhitbox(2,charpos,bosspos);
    }
    else if(bstance == &bosscrack && timer(300,&btag)){
        bstance = &bossstand;
        status = 0;
    }
}

void area(pos charpos,pos bosspos){
    if(bstance == &bossstand && timer(500,&btag)){
        bstance = &bossprearea;
    }
    else if(bstance == &bossprearea && timer(750,&btag)){
        bstance = &bossarea;
        scanhitbox(3,charpos,bosspos);
    }
    else if(bstance == &bossarea && timer(250,&btag)){
        bstance = &bossstand;
        status = 0;
    }
}

void poke(pos charpos,pos bosspos){
    if(bstance == &bossstand && timer(500, &btag)){
        bstance = &bossprepoke;
    }
    else if(bstance == &bossprepoke && timer(500,&btag)){
        bstance = &bosspoke;
        scanhitbox(4,charpos,bosspos);
    }
    else if(bstance == &bosspoke && timer(250,&btag)){
        bstance = &bossstand;
        status = 0;
    }
}