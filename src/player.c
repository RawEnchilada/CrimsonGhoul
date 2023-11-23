#include "gfx.h"
#include "main.h"
#include <stdlib.h>
#include <debugmalloc.h>

void jump(SDL_bool* inair,SDL_bool *wp,pos *charpos);
void sneak(SDL_bool *sn, int *h,SDL_bool* rolled, SDL_bool* inair);
void run(SDL_bool* running,int *h,pos *charpos);
void attack(SDL_bool *ap,SDL_bool* slash,pos*charpos,pos*bosspos);
void boundaries(pos charpos, pos bosspos);

SDL_bool face = SDL_TRUE;
SDL_Texture* *cstance = &charstand;

SDL_bool timer(int delay,Uint32 *tagp){
    Uint32 ticks = SDL_GetTicks();
    int passed = ticks - *tagp;
    if(passed >= delay || *tagp == -1){
        *tagp = ticks;
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

//main function running and initializing the game
void mainfunc(SDL_bool* screenrunning){
    condition = 0;

    file = fopen("./stats.txt","w");

    fprintf(file,"smash_kills\n%d\nstomp_kills\n%d\ncrack_kills\n%d\narea_kills\n%d\npoke_kills\n%d\n---\n",kills[0],kills[1],kills[2],kills[3],kills[4]);

    SDL_Event event;

    SDL_bool inair = SDL_FALSE;
    SDL_bool running = SDL_FALSE;
    SDL_bool rolled = SDL_FALSE;
    SDL_bool slash = SDL_FALSE;
    SDL_bool wp = SDL_FALSE;
    SDL_bool crouch = SDL_FALSE;
    SDL_bool ap = SDL_FALSE;

    SDL_bool inloop = SDL_TRUE;


    SDL_Rect btn1 = {257,230,126,27};
    SDL_Rect btn2 = {257,260,126,27};

    tag = -1;
    bosshp = 150;
    playerhit = SDL_FALSE;
    int h = 0;

	pos charpos={100,300};
	pos bosspos={300,32};
    cstance = &charstand;
    status = 0;
    bstance = &bossstand;
    bossface = SDL_FALSE;
    face = SDL_TRUE;

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    while(inloop){
        if(condition == 0) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case (SDL_QUIT):
                        *screenrunning = SDL_FALSE;
                        inloop = SDL_FALSE;
                        condition = 3;
                        break;
                }
                if(keystates[SDL_SCANCODE_ESCAPE])inloop = SDL_FALSE;
                if (keystates[SDL_SCANCODE_A]) {
                    face = SDL_FALSE;
                    if (abs(h) < 6 && charpos.x > 3)h = -2;
                } else if (keystates[SDL_SCANCODE_D]) {
                    if (abs(h) < 6 && charpos.x < 637)h = 2;
                    face = SDL_TRUE;
                } else h = 0;
                if (keystates[SDL_SCANCODE_W])wp = SDL_TRUE;
                else wp = SDL_FALSE;
                if (keystates[SDL_SCANCODE_S]){
                    crouch = SDL_TRUE;
                    wp = SDL_FALSE;
                }
                else {
                    crouch = SDL_FALSE;
                    rolled = SDL_FALSE;
                }
                if (keystates[SDL_SCANCODE_K])ap = SDL_TRUE;
                else ap = SDL_FALSE;
            }
            run(&running, &h, &charpos);
            jump(&inair, &wp,&charpos);
            sneak(&crouch, &h, &rolled, &inair);
            attack(&ap, &slash,&charpos,&bosspos);

            boss(&bosspos,charpos);

            boundaries(charpos, bosspos);

            render(charpos, bosspos);
        }
        if(condition != 0 && *screenrunning){
            endgame(&inloop,event,btn1,btn2,screenrunning);
        }
    }
    fclose(file);
}

void run(SDL_bool* running,int *h,pos *charpos){
    if(*h != 0) {
        charpos->x += *h;
        if(!(cstance == &charjump || cstance == &charfall)) {
            if(*running == SDL_FALSE && timer(200,&tag)){
                cstance = &charrun1;
                *running = SDL_TRUE;
            }
            else if(*running == SDL_TRUE && timer(200,&tag)){
                cstance = &charrun2;
                *running = SDL_FALSE;
            }
        }
    }
    else if(cstance == &charrun1 || cstance == &charrun2){
        cstance = &charstand;
        *running = SDL_FALSE;
    }
}
void jump(SDL_bool* inair,SDL_bool *wp,pos *charpos){
    if (charpos->y > 150 && !*inair && *wp) {
        charpos->y -= 6;
        cstance = &charjump;
    }
    else if (charpos->y <= 150 || (!*wp && charpos->y < 300))*inair = SDL_TRUE;
    if (charpos->y < 300 && *inair) {
        cstance = &charfall;
        charpos->y += 5;
        if (charpos->y >= 300) {
            *inair = SDL_FALSE;
            if(cstance == &charfall)cstance = &charstand;
            *wp = SDL_FALSE;
        }
    }
}
void sneak(SDL_bool *sn, int *h,SDL_bool* rolled, SDL_bool* inair){
    if(cstance == &charroll1 && timer(50, &tag)){
        cstance = &charroll2;
    }
    else if(cstance == &charroll2 && timer(150, &tag)){
        if(abs(*h) == 6)*h /= 3;
        cstance = &charroll3;
        *rolled = SDL_TRUE;
    }
    if(*sn) {
        if(*h != 0 && !*rolled){
            if(!(cstance == &charroll1 || cstance == &charroll2) && !*inair){
                cstance = &charroll1;
                if(abs(*h)==2)*h *= 3;
            }
        }
        else if ((cstance == &charstand || cstance == &charroll3) && *h == 0)cstance = &charcrouch;
    }
    else if(cstance == &charcrouch || (cstance == &charroll3))cstance = &charstand;
}
void attack(SDL_bool *ap,SDL_bool* slash,pos*charpos,pos*bosspos){
    if(*ap && !*slash){
        cstance = &charattack;
        *slash = SDL_TRUE;
        scanhitbox(5,*charpos,*bosspos);
    }
    if(*slash && timer(200,&tag)) {
        cstance = &charstand;
        *slash = SDL_FALSE;
    }

}

void boundaries(pos charpos, pos bosspos){
    if(charpos.x < 0)charpos.x = 0;
    if(charpos.x > (640-32))charpos.x = (640-32);
    if(bosspos.x+75 < 0)bosspos.x = -75;
    if(bosspos.x+225 > 640)bosspos.x = (640-225);
}
