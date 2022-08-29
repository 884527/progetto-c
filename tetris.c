#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <termmgr.h>
struct tetris {
    char **game;
    int wd;     /** larghezza */
    int hg;     /** altezza */
    int score;
    int gameover;
    struct tetris_block {
        char data[5][5];
        int wd;     /** larghezza */
        int hg;     /** altezza */
    } current;
    int x;
    int y;
};

struct tetris_block blocks[] =
        {
                {{"##",
                           "##"},
                        2, 2
                },
                {{" X ",
                           "XXX"},
                        3, 2
                },
                {{"@@@@"},
                        4, 1},
                {{"OO",
                           "O ",
                           "O "},
                        2, 3},
                {{"&&",
                           " &",
                           " &"},
                        2, 3},
                {{"ZZ ",
                         " ZZ"},
                        3, 2}
        };

#define TETRAMINI (sizeof(blocks)/sizeof(struct tetris_block))

void init (struct tetris *t) {
    t->score = 0;
    t->wd = 10;        /** 10 + 9 spazi */
    t->hg = 15;
    t->game = malloc(sizeof(char *)*t->wd);
    t->gameover=0;
    int x, y;
    for(x=0;x<10;x++) {
        t->game[x] = malloc(sizeof(char) * t->hg);
        for (y = 0; y < 15; y++)
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
    printf("[SCORE: %d]\n", t->score);
    for(x=0;x<t->wd+2;x++)
        printf("~");
    printf("\n");

    for(y=0;y<t->hg;y++) {
        printf("|");
        for(x=0;x<t->wd;x++)
            if(x>t->x && x<(t->x + t->current.wd) && y<(t->y + t->current.hg) && t->current.data[y-t->y][x-t->x] != ' ')
                printf("%c", t->current.data[y-t->y][x-t->x]);
            else
                printf("%c", t->game[x][y]);
        printf("|\n");
    }

    for(x=0;x<t->wd+2;x++)
        printf("~");
    printf("\n");
}

int hittest (struct tetris *t) {
    int x, y, xx, yy;
    struct tetris_block b = t->current;
    for(x=0;x<b.wd;x++) {
        for(y=0;y<b.hg;y++) {
            xx = t->x +x;
            yy = t->y +y;
            if(xx<0 || xx>=t->wd)
                return 1;
            if(b.data[y][x]!=' ' && (yy>=t->hg  || (xx<t->wd && t->game[xx][yy] != ' ')))  //Controllo se c'è un altro blocco
                return 1;
        }
    }
    return 0;
}

void new_block (struct tetris *t) {
    t->current = blocks [rand()%TETRAMINI];
    t->x = (t->wd/2) - (t->current.wd/2); //centro
    t->y=0; //prima riga

    if(hittest(t))
        t->gameover=1;
}
void prova (struct tetris *t){
    t->current = blocks[0];
    t->x = (t->wd/2) - (t->current.wd/2); //centro
    t->y=0; //prima riga

    //gsmeover
}

void print_block (struct tetris *t) {
    int x, y;
    struct tetris_block b = t->current;
    for(x=0;x<b.wd;x++) {
        for(y=0;y<b.hg;y++) {
            if(b.data[y][x]!=' ')
                t->game[t->x+x][t->y+y] = b.data[y][x];
        }
    }
}

void gravity (struct tetris *t) {
    int x, y;
    t->y++;
    if(hittest(t)) {
        t->y--;
        print_block(t);
        new_block(t);
    }
}

void fall (struct tetris *t, int l) {
    int x, y;
    for(y=l;y>0;y--) {
        for(x=0;x<t->wd;x++)
            t->game[x][y] = t->game[x][y-1];
    }
    for(x=0;x<t->wd;x++)
        t->game[x][0] = ' ';
}

void rotate (struct tetris *t) {

}

void check_lines (struct tetris *t) {
    int x, y, l;
    int p=100;
    for(y=t->hg-1;y>=0;y--) {
        l=1;
        for(x=0;x<t->wd;x+=2) {
            if (t->game[x][y] == ' ')
                l = 0;
        }
        if(l) {
            t->score += p;
            fall(t,y);
            y++;
        }
    }
}
char chiediMossa()  {
    char cmd;
    printf("A = sinistra \n D = destra \n S = Giu \n  spazio = rilascia \n R = Ruota a destra \n");
    cmd= getchar();
    return cmd;
}

void run () {
    struct tetris t;
    char cmd;
    int count=0;
    init(&t);
    new_block(&t);
    while (t.gameover==0) {
        count++;
        if (count%50 == 0) {
            print(&t);
        }
        if (count%350 == 0) {
            gravity(&t);
            check_lines(&t);
        }
        cmd=chiediMossa();
        print(&t);

            switch (cmd) {
                case 'a':
                    t.x--;
                    if (hittest(&t))
                        t.x++;
                    break;
                case 'd':
                    t.x++;
                    if (hittest(&t))
                        t.x--;
                    break;
                case 's':
                    gravity(&t);
                    break;
                case 'r':
                    rotate(&t);
                    break;
            }
        }


    print(&t);
    printf(" GAME OVER \n");

    clean(&t);
}
