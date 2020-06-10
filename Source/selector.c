#include <debugmalloc.h>
#include<time.h>
#include "gfx.h"
#include "main.h"
#include "boss.h"
#include <string.h>




void selector(int* dist, int* abl) {
    srand(time(NULL) + rand());

    int n = rand() % 100;


    if (n <= rat[0]){
        *dist = 125;
        *abl = 0;
    }
    else if(rat[0] < n && n <= (rat[0]+rat[1])){
        *dist = 75;
        *abl = 1;
    }
    else if(rat[0]+rat[1] < n && n <= (rat[0]+rat[1]+rat[2])){
        *dist = 300;
        *abl = 2;
    }
    else if(rat[0]+rat[1]+rat[2] < n && n <= (rat[0]+rat[1]+rat[2]+rat[3])){
        *dist = 75;
        *abl = 3;
    }
    else if(rat[0]+rat[1]+rat[2]+rat[3] < n && n <= 100){
        *dist = 75;
        *abl = 4;
    }
}

SDL_bool addtolists(node *lists[5]){
    FILE *fp = fopen("sources/stats.txt","r");
    if(fp == NULL)return SDL_FALSE;
    for (int i = 0; i < 5; ++i) {
        lists[i] = (node*)calloc(sizeof(node),1);
    }

    char* line = NULL;
    size_t len = 0;
    node* smash = lists[0];
    node* stomp = lists[1];
    node* crack = lists[2];
    node* area = lists[3];
    node* poke = lists[4];
    SDL_bool init = SDL_TRUE;

    while((getline(&line,&len,fp)) != -1){
        while(init){
            if(strcmp(line,"smash_kills\n") == 0){
                getline(&line,&len,fp);
                kills[0] = atoi(line);
                //debug
                printf("smashk %s\n",line);
            }
            else if(strcmp(line,"stomp_kills\n") == 0){
                getline(&line,&len,fp);
                kills[1] = atoi(line);
                //debug
                printf("stompk %s\n",line);
            }
            else if(strcmp(line,"crack_kills\n") == 0){
                getline(&line,&len,fp);
                kills[2] = atoi(line);
                //debug
                printf("crackk %s\n",line);
            }
            else if(strcmp(line,"area_kills\n") == 0){
                getline(&line,&len,fp);
                kills[3] = atoi(line);
                //debug
                printf("areak %s\n",line);
            }
            else if(strcmp(line,"poke_kills\n") == 0){
                getline(&line,&len,fp);
                kills[4] = atoi(line);
                //debug
                printf("pokek %s\n",line);
            }
            else if(strcmp(line,"---\n") == 0){
                init = SDL_FALSE;
                printf("Found end of init\n");
            }
            else {
                printf("Corrupted file, delete stats.txt\n");
                return SDL_FALSE;
            }
            getline(&line,&len,fp);
        }
        if (strcmp(line, "smash\n") == 0) {
            smash->next = (node*)calloc(sizeof(node),1);
            smash = smash->next;
            getline(&line, &len, fp);
            if(atoi(line) == 1)kills[0]++;
            else
                smash->val = atoi(line);
            //debug
            printf("smash %s\n", line);
        } else if (strcmp(line, "stomp\n") == 0) {
            stomp->next = (node*)calloc(sizeof(node),1);
            stomp = stomp->next;
            getline(&line, &len, fp);
            if(atoi(line) == 1)kills[1]++;
            else
                stomp->val = atoi(line);
            //debug
            printf("stomp %s\n", line);
        } else if (strcmp(line, "crack\n") == 0) {
            crack->next = (node*)calloc(sizeof(node),1);
            crack = crack->next;
            getline(&line, &len, fp);
            if(atoi(line) == 1)kills[2]++;
            else
                crack->val = atoi(line);
            //debug
            printf("crack %s\n", line);
        } else if (strcmp(line, "area\n") == 0) {
            area->next = (node*)calloc(sizeof(node),1);
            area = area->next;
            getline(&line, &len, fp);
            if(atoi(line) == 1)kills[3]++;
            else
                area->val = atoi(line);
            //debug
            printf("area %s\n", line);
        } else if (strcmp(line, "poke\n") == 0) {
            poke->next = (node*)calloc(sizeof(node),1);
            poke = poke->next;
            getline(&line, &len, fp);
            if(atoi(line) == 1)kills[4]++;
            else
                poke->val = atoi(line);
            //debug
            printf("poke %s\n", line);
        }
        smash->next = NULL;
        stomp->next = NULL;
        crack->next = NULL;
        area->next = NULL;
        poke->next = NULL;
    }
    fclose(fp);
    return SDL_TRUE;
}

void setratio(node** head){
    int sum[5] = {0,0,0,0,0};
    int cnt[5] = {0,0,0,0,0};

    for (int j = 0; j < 5; ++j) {
        node *current = head[j];
        while (current != NULL) {
            sum[j] += current->val;
            cnt[j]++;
            current = current->next;
        }
    }
    double stat[5] = {0,0,0,0,0};
    int avg[5] = {0,0,0,0,0};
    int tavg = 0;
    double tempsum = 0;
    for (int i = 0; i < 5; ++i) {
        printf("Kills:%d / Count:%d /avg:",kills[i],cnt[i]);
        if(cnt[i] == 0)cnt[i] = 1;
        if(sum[i] == 0)sum[i] = 10;
        avg[i] = (sum[i] / cnt[i]);
        printf(" %d\n",avg[i]);
        if(avg[i] == 0)avg[i] = 100;
        tavg += avg[i];
    }
    if(tavg == 0)tavg = 100;
    for (int k = 0; k < 5; ++k) {//convert to percentage
        if(kills[k] != 0)
        stat[k] = ((double) kills[k]*(tavg/avg[k]));
        else{
            stat[k] = ((double) (tavg/avg[k]));
        }
        tempsum += stat[k];
    }
    for (int l = 0; l < 5; ++l) {
        rat[l] = (int) (stat[l] * 100 / tempsum);
        if(rat[l] == 0)rat[l] = 5;
    }
    printf("RATIO: %d/%d/%d/%d/%d\n",rat[0],rat[1],rat[2],rat[3],rat[4]);
}


void freelist(node** head){
    for (int i = 0; i < 5; ++i) {
        node *current = head[i];
        node *tobe = head[i];
        while (current != NULL) {
            current = current->next;
            free(tobe);
            tobe = current;
        }
    }
}

