#include "gfx.h"
#include "main.h"
#include <debugmalloc.h>

SDL_Surface** spritesheetc;
SDL_Surface** spritesheetb;
SDL_Texture* menuframe[3];
SDL_Texture* buttonstxt[8];
SDL_Texture* controls;
SDL_Texture* lose;
SDL_Texture* victory;
SDL_Surface* darkensrf;
SDL_Surface* controlssrf;
SDL_Surface* losesrf;
SDL_Surface* victorysrf;
int n = 0;

SDL_bool over(pos temp, SDL_Rect dest){
    if(!((temp.x > dest.x*2+dest.w*2) ||
         (temp.x < dest.x*2) ||
         (temp.y > dest.y*2+dest.h*2) ||
         (temp.y < dest.y*2)))return SDL_TRUE;
    else return SDL_FALSE;
}

void sprites(){
    charstand = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[1]);
    charrun1 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[2]);
    charrun2 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[3]);
    charjump = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[4]);
    charfall = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[5]);
    charcrouch = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[6]);
    charroll1 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[7]);
    charroll2 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[8]);
    charroll3 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[9]);
    charattack = SDL_CreateTextureFromSurface(screen.renderer, spritesheetc[10]);

    bossstand = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[1]);
    bosssmash = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[2]);
    bosspresmash = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[3]);
    bossprestomp = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[4]);
    bossstomp = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[5]);
    bossprecrack = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[6]);
    bosscrack = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[7]);
    bossprearea = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[8]);
    bossarea = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[9]);
    bossprepoke = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[10]);
    bosspoke = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[11]);
    rock = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[12]);
    bossstep1 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[13]);
    bossstep2 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[14]);
    bossstep3 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[15]);
    bossstep4 = SDL_CreateTextureFromSurface(screen.renderer, spritesheetb[16]);

    darken = SDL_CreateTextureFromSurface(screen.renderer, darkensrf);
    SDL_SetTextureBlendMode(darken,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(darken, 150);
    controls = SDL_CreateTextureFromSurface(screen.renderer, controlssrf);

    lose = SDL_CreateTextureFromSurface(screen.renderer, losesrf);
    victory = SDL_CreateTextureFromSurface(screen.renderer, victorysrf);
}

void init(){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        printf("SDL_Failed: %s",SDL_GetError());
        exit(1);
    }

    screen.w = 1280;
    screen.h = 720;
    screen.window = SDL_CreateWindow(
            "Crimson Ghoul",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screen.w, screen.h, 0
    );
    screen.renderer = SDL_CreateRenderer(
            screen.window, -1,
            SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetScale(screen.renderer,2,2);
    SDL_SetRenderDrawBlendMode(screen.renderer, SDL_BLENDMODE_ADD);

    SDL_SetRenderDrawColor(screen.renderer,0,0,0,0);
    SDL_Surface* charsrfc = SDL_LoadBMP("assets/character.bmp");
    SDL_Surface* background = SDL_LoadBMP("assets/background.bmp");
    SDL_Surface* bosssrfc = SDL_LoadBMP("assets/boss.bmp");
    SDL_Surface* hpbarsurf = SDL_LoadBMP("assets/Healthbar.bmp");
    SDL_Surface* menusrf = SDL_LoadBMP("assets/menu.bmp");
    SDL_Surface* buttonsrf = SDL_LoadBMP("assets/buttons.bmp");
    victorysrf = SDL_LoadBMP("assets/victory.bmp");
    losesrf = SDL_LoadBMP("assets/lose.bmp");

    controlssrf = SDL_LoadBMP("assets/controls.bmp");
    darkensrf = SDL_CreateRGBSurface(0,640,380,24,0x00,0x0,0x0,0x80);

    SDL_Surface* framesrfc = SDL_CreateRGBSurface(0,154,10,24,0x00,0x00,0x00,0x00);
    SDL_Surface* hpsrfc = SDL_CreateRGBSurface(0,150,6,24,0x00,0x00,0x00,0x00);

    SDL_SetColorKey(framesrfc,1,0xFF00FF);
    SDL_SetColorKey(hpsrfc,1,0xFF00FF);
    SDL_SetColorKey(controlssrf,1,0xFF00FF);
    SDL_SetColorKey(losesrf,1,0xFF00FF);
    SDL_SetColorKey(victorysrf,1,0xFF00FF);

    SDL_Rect box = {2,2,150,6};
    SDL_BlitSurface(hpbarsurf,&box,hpsrfc,NULL);
    box.y = 10;box.x = 0;box.w = 154;box.h = 10;
    SDL_BlitSurface(hpbarsurf,&box,framesrfc,NULL);

    int gfxn = ((charsrfc->w / 32) * (charsrfc->h / 32) + 1);



    spritesheetc = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*gfxn);

    int i, x, y;
    SDL_Rect rect = {0, 0, 32, 32};
    for(i=0; i<gfxn; i++) {
        spritesheetc[i] = SDL_CreateRGBSurface(0, 32, 32, 24, 0x00, 0x00, 0x00, 0x00);
        SDL_SetColorKey(spritesheetc[i], 1, 0xFF00FF);
        SDL_FillRect(spritesheetc[i], 0, 0xFF00FF);
        if(i!=0) {
            x = (i-1)%(charsrfc->w / 32);
            y = (i-x)/(charsrfc->w / 32);
            rect.x = x*32;
            rect.y = y*32;
            SDL_BlitSurface(charsrfc, &rect, spritesheetc[i], NULL);
        }
    }

    gfxn = ((bosssrfc->w/150)*(bosssrfc->h/150)+1);
    spritesheetb = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*gfxn);
    rect.x = 0; rect.y = 0;rect.w = 150; rect.h = 150;
    for(i=0; i<gfxn; i++) {
        spritesheetb[i] = SDL_CreateRGBSurface(0, 150, 150, 24, 0x00, 0x00, 0x00, 0x00);
        SDL_SetColorKey(spritesheetb[i], 1, 0xFF00FF);
        SDL_FillRect(spritesheetb[i], 0, 0xFF00FF);
        if(i!=0) {
            x = (i-1)%(bosssrfc->w / 150);
            y = (i-x)/(bosssrfc->w / 150);
            rect.x = x*150;
            rect.y = y*150;
            SDL_BlitSurface(bosssrfc, &rect, spritesheetb[i], NULL);
        }
    }

    SDL_Surface** wallpaper = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*4);
    rect.x = 0; rect.y = 0;rect.w = 640; rect.h = 360;
    for(i=0; i<4; i++) {
        wallpaper[i] = SDL_CreateRGBSurface(0, 640, 360, 24, 0x00, 0x00, 0x00, 0x00);
        SDL_SetColorKey(wallpaper[i], 1, 0xFF00FF);
        SDL_FillRect(wallpaper[i], 0, 0xFF00FF);
        SDL_BlitSurface(menusrf, &rect, wallpaper[i], NULL);
        rect.x += 640;
    }

    SDL_Surface** mbuttons = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*9);
    rect.x = 0; rect.y = 0;rect.w = 126; rect.h = 27;
    for(i=0; i<8; i++) {
        mbuttons[i] = SDL_CreateRGBSurface(0, 126, 27, 24, 0x00, 0x00, 0x00, 0x00);
        SDL_SetColorKey(mbuttons[i], 1, 0xFF00FF);
        SDL_FillRect(mbuttons[i], 0, 0xFF00FF);
        SDL_BlitSurface(buttonsrf, &rect, mbuttons[i], NULL);
        rect.x += 126;

        if(i == 3){
            rect.y = 27;
            rect.x = 0;
        }
    }

    for(i = 0;i < 3;i++) {
        menuframe[i] = SDL_CreateTextureFromSurface(screen.renderer, wallpaper[i]);
    }
    for(i = 0;i < 8;i++) {
        buttonstxt[i] = SDL_CreateTextureFromSurface(screen.renderer, mbuttons[i]);
    }
    wall = SDL_CreateTextureFromSurface(screen.renderer, background);
    hpbar = SDL_CreateTextureFromSurface(screen.renderer, hpsrfc);
    hpframe = SDL_CreateTextureFromSurface(screen.renderer, framesrfc);

    sprites();
    SDL_FreeSurface(charsrfc);
    SDL_FreeSurface(background);
    SDL_FreeSurface(bosssrfc);
    SDL_FreeSurface(hpsrfc);
    SDL_FreeSurface(hpbarsurf);
    SDL_FreeSurface(menusrf);
    SDL_FreeSurface(buttonsrf);
    SDL_FreeSurface(controlssrf);
    SDL_FreeSurface(framesrfc);
    SDL_FreeSurface(darkensrf);
    SDL_FreeSurface(losesrf);
    SDL_FreeSurface(victorysrf);
    for (i = 0; i < 17; i++) {
        SDL_FreeSurface(spritesheetb[i]);
    }
    free(spritesheetb);
    for (i = 0; i < 11; i++) {
        SDL_FreeSurface(spritesheetc[i]);
    }
    free(spritesheetc);
    for (i = 0; i < 4; i++) {
        SDL_FreeSurface(wallpaper[i]);
    }
    free(wallpaper);
    for (i = 0; i < 6; i++) {
        SDL_FreeSurface(mbuttons[i]);
    }
    free(mbuttons);

}



void render(pos charpos,pos bosspos){
    SDL_RenderClear(screen.renderer);

    SDL_Rect bckgrnd = {0,0,640,360};
    SDL_RenderCopy(screen.renderer,wall,NULL,&bckgrnd);

    SDL_Rect bossrect = {bosspos.x,bosspos.y,300,300};
    SDL_Rect charrect = {charpos.x,charpos.y,32,32};
	SDL_Rect rockrect = {charpos.x-59,182,150,150};

    if(bstance == &bosscrack)SDL_RenderCopy(screen.renderer,rock,NULL,&rockrect);

    if(!bossface)
        SDL_RenderCopy(screen.renderer, *bstance, NULL, &bossrect);
    else
        SDL_RenderCopyEx(screen.renderer, (*bstance), NULL, &bossrect, 0, NULL, SDL_FLIP_HORIZONTAL);


    if(face)
        SDL_RenderCopy(screen.renderer, (*cstance), NULL, &charrect);
    else
        SDL_RenderCopyEx(screen.renderer, (*cstance), NULL, &charrect, 0, NULL, SDL_FLIP_HORIZONTAL);

    SDL_Rect bar = {(screen.w/4)-77,5,154,10};
    SDL_Rect percent = {(screen.w/4)-75,7,bosshp,6};
    SDL_RenderCopy(screen.renderer,hpframe,NULL,&bar);
    SDL_RenderCopy(screen.renderer, hpbar,NULL,&percent);

    SDL_RenderPresent(screen.renderer);
}


void endgame(SDL_bool *inloop,SDL_Event event,SDL_Rect btn1,SDL_Rect btn2,SDL_bool* screenrunning){
    int p;
    pos mouse;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    SDL_Rect a = {170,60,300,150};
    SDL_RenderCopy(screen.renderer,darken,NULL,NULL);
    switch(condition){
        case(2):
            SDL_RenderCopy(screen.renderer,victory,NULL,&a);
            break;
        case(1):
            SDL_RenderCopy(screen.renderer,lose,NULL,&a);
            break;
    }

    while(*inloop) {
        SDL_GetMouseState(&mouse.x, &mouse.y);
        SDL_RenderCopy(screen.renderer, buttonstxt[6], NULL, &btn1);
        SDL_RenderCopy(screen.renderer, buttonstxt[4], NULL, &btn2);


        if (over(mouse, btn1)) {
            SDL_RenderCopy(screen.renderer, buttonstxt[7], NULL, &btn1);
            p = 1;
        } else if (over(mouse, btn2)) {
            SDL_RenderCopy(screen.renderer, buttonstxt[5], NULL, &btn2);
            p = 2;
        } else p = 0;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case (SDL_QUIT):
                    *inloop = SDL_FALSE;
                    retry = SDL_FALSE;
                    *screenrunning = SDL_FALSE;
                    break;
                case (SDL_MOUSEBUTTONDOWN):
                    if (p == 1) {
                        *inloop = SDL_FALSE;
                        retry = SDL_TRUE;
                    }
                    if (p == 2) {
                        *inloop = SDL_FALSE;
                        retry = SDL_FALSE;
                    }
                    break;
            }
            if(keystates[SDL_SCANCODE_SPACE]){
                *inloop = SDL_FALSE;
                retry = SDL_TRUE;
            }
        }

        SDL_RenderPresent(screen.renderer);
    }
}

void rendermenu(int press,SDL_Rect btn,SDL_bool showcontrols){
    SDL_RenderClear(screen.renderer);
    SDL_Rect bckgrnd = {0,0,640,360};
    static int nstate;


    if(n == 0 && timer(3000,&tag)){
        n = 1;
    }
    else if((n == 1 && timer(200,&tag))){
        n = 2;
        if(nstate == 1){
            nstate = 2;
            n = 2;
        }
        else if(nstate == 3){
            n = 0;
            nstate = 0;
        }
    }
    else if(n == 2 && timer(200,&tag)){

        if(nstate == 0){
            nstate = 1;
            n = 1;
        }
        else if(nstate == 2){
            nstate = 3;
            n = 1;
        }
    }

    SDL_RenderCopy(screen.renderer,menuframe[n],NULL,&bckgrnd);

    if(press == 0)SDL_RenderCopy(screen.renderer,buttonstxt[1],NULL,&btn);
    else{SDL_RenderCopy(screen.renderer,buttonstxt[0],NULL,&btn);}
    btn.y += 30;
    if(press == 1)SDL_RenderCopy(screen.renderer,buttonstxt[3],NULL,&btn);
    else{SDL_RenderCopy(screen.renderer,buttonstxt[2],NULL,&btn);}
    btn.y += 30;
    if(press == 2)SDL_RenderCopy(screen.renderer,buttonstxt[5],NULL,&btn);
    else{SDL_RenderCopy(screen.renderer,buttonstxt[4],NULL,&btn);}

    if(showcontrols){
        SDL_Rect rect = {60,30,520,300};
        SDL_RenderCopy(screen.renderer,darken,NULL,NULL);
        SDL_RenderCopy(screen.renderer,controls,NULL,&rect);
    }
    SDL_RenderPresent(screen.renderer);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void quit(SDL_bool* screenrunning) {
    *screenrunning = SDL_FALSE;

    SDL_DestroyRenderer(screen.renderer);
    screen.renderer = NULL;

    SDL_DestroyWindow(screen.window);
    screen.window = NULL;

    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    SDL_Quit();
}