//
// Created by Martina Feltre on 23/08/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

struct tetris {
    char **game;
    int wd;     /**larghezza**/
    int hg;     /**altezza**/
    int score;
    struct tetris_block {
        char data[5][5];
        int wd;     /**larghezza**/
        int hg;     /**altezza**/
    } current;
    int x;
    int y;
};

void init (struct tetris *t, int w, int h) {
    t->score = 0;
    t->wd = w;
    t->hg = h;
    t->game = malloc(sizeof(char *)*w);
    int x, y;
    for(x=0;x<w;x++) {
        t->game[x] = malloc(sizeof(char) * h);
        for (y = 0; y < h; y++)
            t->game[x][y] = ' ';
    }
}

void clean (struct  tetris *t) {
    int x;
    for(x=0;x<t->wd;x++)
        free(t->game[x]);
    free(t->game);
}

void print (struct tetris *t) {
    int x, y;
    for(x=0;x<30;x++)
        printf("\n");
    printf("SCORE: %d\n", t->score);
    for(x=0;x<2*t->wd+2;x++)
        printf("~");
    printf("\n");

    for(y=0;y<t->hg;y++){
        printf("!");
        for(x=0;x<t->wd;x++) {
            if(x>t->x
                && x<(t->x + t->current.wd) && y<(t->y + t->current.hg)
                && t->current.data[y-t->y][x-t->x] != ' ')
                printf("%c", t->current.data[y-t->y][x-t->x]);
            else
                printf("%c", t->game[x][y]);
        }
        printf("!\n)");
    }
    for(x=0;x<2*t->wd+2;x++)
        printf("~");
    printf("\n");
}

void rotate (struct tetris *t) {

}

void check_lines (struct tetris *t) {

}