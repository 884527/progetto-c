#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

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
        int n;
    } current;
    int x;
    int y;
};

struct tetris_block blocks[] =
        {
                {{"##",
                         "##"},
                        2, 2, 20
                },
                {{" X ",
                         "XXX"},
                        3, 2, 20
                },
                {{"@@@@"},
                        4, 1, 20},
                {{"OO",
                         "O ",
                         "O "},
                        2, 3, 20},
                {{"ZZ ",
                         " ZZ"},
                        3, 2, 20}
        };

#define TETRAMINI (sizeof(blocks)/sizeof(struct tetris_block))

/**
 * inizializza il campo di gioco
 */
void init (struct tetris *t) {
    t->score = 0;
    t->wd = 10;
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

/**
 * pulisce il campo di gioco
 */
void clean (struct  tetris *t) {
    int x;
    for(x=0;x<t->wd;x++)
        free(t->game[x]);
    free(t->game);
}

/**
 * stampa il campo di gioco e il punteggio
 */
void print (struct tetris *t) {
    int x, y;
    printf("[SCORE: %d]\n", t->score);
    for(x=0;x<t->wd+2;x++)
        printf("~");
    printf("\n");

    for(y=0;y<t->hg;y++) {
        printf("|");
        for(x=0;x<t->wd;x++)
            if(x>=t->x && y>=t->y
               && x<(t->x + t->current.wd) && y<(t->y + t->current.hg)
               && t->current.data[y-t->y][x-t->x] != ' ')
                printf("%c", t->current.data[y-t->y][x-t->x]);
            else
                printf("%c", t->game[x][y]);
        printf("|\n");
    }

    for(x=0;x<t->wd+2;x++)
        printf("~");
    printf("\n");
}

/**
 * controlla quando il blocco corrente tocca gli altri blocchi nel campo oppure i confini del campo
 */
int hittest (struct tetris *t) {
    int x, y, xx, yy;
    struct tetris_block b = t->current;
    for(x=0;x<b.wd;x++) {
        for(y=0;y<b.hg;y++) {
            xx = t->x +x;
            yy = t->y +y;
            if(xx<0 || xx>=t->wd)
                return 1;
            /** controllo se c'è un altro blocco */
            if(b.data[y][x]!=' ' && (yy>=t->hg  || (xx<t->wd && t->game[xx][yy] != ' ')))
                return 1;
        }
    }
    return 0;
}

/**
 * genera un nuovo blocco (in modo random) controllando il numero di blocchi: 20 blocchi per ogni tipo
 */
void new_block (struct tetris *t) {
    int exit=0, x, tot=20*5;
    while(exit==0 && tot>0) {
        x = rand() % TETRAMINI;
        if (blocks[x].n > 0) {
            t->current = blocks[x];
            blocks[x].n--;
            tot--;
            exit=1;
        }
    }
    t->x = (t->wd/2) - (t->current.wd/2); //centro
    t->y=0; //prima riga

    if(hittest(t))
        t->gameover=1;
    if(tot==0) {
        t->gameover=1;
        printf("Hai terminato i blocchi!\n");
    }
}

/**
 * stampa il blocco
 */
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

/**
 * fa scendere il blocco
 */
void gravity (struct tetris *t) {
    int x, y;
    t->y+=1;
    if(hittest(t)) {
        t->y--;
        print_block(t);
        new_block(t);
    }
}

/**
 * ruota il blocco di 90 gradi
 */
void rotate (struct tetris *t) {
    struct tetris_block b=t->current;
    struct tetris_block s=b;
    int x,y;
    b.wd=s.hg;
    b.hg=s.wd;
    for (x=0; x<s.wd; x++)
        for (y=0; y<s.hg; y++) {
            b.data[x][y]=s.data[s.hg-y-1][x];
        }
    x=t->x;
    y=t->y;
    t->x-=(b.wd-s.wd)/2;
    t->y-=(b.hg-s.hg)/2;
    t->current=b;
    if (hittest(t)) {
        t->current=s;
        t->x=x;
        t->y=y;
    }
}

/**
 * elimina la riga completa e fa scendere le righe superiori
 */
void fall (struct tetris *t, int l) {
    int x, y;
    for(y=l;y>0;y--) {
        for(x=0;x<t->wd;x++)
            t->game[x][y] = t->game[x][y-1];
    }
    for(x=0;x<t->wd;x++)
        t->game[x][0] = ' ';
}

/**
 * controlla se ci sono righe complete da eliminare e nel caso incrementa il punteggio
 */
void check_lines (struct tetris *t) {
    int x, y, l, count=0;
    int p=1;
    for(y=t->hg-1;y>=0;y--) {
        l=1;
        for(x=0;x<t->wd && l;x++) {
            if (t->game[x][y] == ' ') {
                l = 0;
                count=0;
            }
        }
        if (l) {
            count++;
            t->score += p;
            if (count == 2)    /**quando si eliminano 3 righe*/
                p = 3;
            else if (count == 3)     /**quando si eliminano 4 righe*/
                p = 6;
            else
                p *= 2;       /** quando si eliminano 1 o 2 righe */
            fall(t, y);
            y++;
        }
    }
}

/**
 * funzione per chiedere la mossa al giocatore
 */
char chiediMossa()  {
    char cmd;
    printf("Scegli la tua mossa: \na = sinistra \nd = destra \ns = giu' \nr = ruota a destra \n\n");
    scanf(" %c",&cmd);
    return cmd;
}

/**
 * single player
 */
void run () {
    struct tetris t;
    char cmd;
    init(&t);
    new_block(&t);
    while (t.gameover==0) {
        print(&t);
        cmd=chiediMossa();

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
        check_lines(&t);
    }

    print(&t);
    printf(" GAME OVER \n");

    clean(&t);
}


/**
 *     ***MODALITA' MULTI PLAYER***
 */

void new_block_multi (struct tetris *t_gio, struct tetris *t_avv){
    int exit=0, x, tot=40*5;
    while(exit==0 && tot>0) {
        x = rand() % TETRAMINI;
        if (blocks[x].n > 0) {
            t_gio->current = blocks[x];
            blocks[x].n--;
            tot--;
            exit=1;
        }
    }
    t_gio->x = (t_gio->wd/2) - (t_gio->current.wd/2); //centro
    t_gio->y=0; //prima riga

    if(hittest(t_gio))
        t_gio->gameover=1;
    if(tot==0) {
        printf("Blocchi terminati!\n");
        if(t_gio->score >= t_avv->score)
            t_avv->gameover=1;
        else
            t_gio->gameover=1;
    }
}


void check_lines_multi (struct tetris *t_gio, struct tetris *t_avv) {
    int x, y, l, count=0;
    int p=1;
    for(y=t_gio->hg-1;y>=0;y--) {
        l=1;
        for(x=0;x<t_gio->wd && l;x++) {
            if (t_gio->game[x][y] == ' ') {
                l = 0;
                count=0;
            }
        }
        if (l) {
            count++;
            t_gio->score += p;
            if (count == 2) {    /**quando si eliminano 3 righe*/
                p = 3;
                invert_pos(&t_avv, count + 1);
            } else if (count == 3) {    /**quando si eliminano 4 righe*/
                p = 6;
                invert_pos(&t_avv, count + 1);
            }else
                p *= 2;       /** quando si eliminano 1 o 2 righe */
            fall(t_gio, y);
            y++;
        }
    }
}

/**
 * inverte le ultime n righe del campo t: una posizione vuota diventa piena e viceversa
 */
void invert_pos(struct tetris *t, int n) {
    int x, y;
    for(y=t->hg-1;y>t->hg-n;y--) {
        for(x=0;x<t->wd;x++) {
            if(t->game[x][y]== ' ')
                t->game[x][y] = 'X';
            else
                t->game[x][y] = ' ';
        }
    }
}

/**
 * multiplayer
 */
void run_multi() {
    struct tetris t1, t2;
    char cmd1, cmd2;
    init(&t1);
    init(&t2);
    new_block_multi(&t1, &t2);
    new_block_multi(&t2, &t1);

    while (t1.gameover==0 && t2.gameover==0) {
        int c=15;
        while (c>0) {
            printf("\nGIOCATORE 1\n");
            print(&t1);
            cmd1 = chiediMossa();

            switch (cmd1) {
                case 'a':
                    t1.x--;
                    if (hittest(&t1))
                        t1.x++;
                    break;
                case 'd':
                    t1.x++;
                    if (hittest(&t1))
                        t1.x--;
                    break;
                case 's':
                    gravity(&t1);
                    c--;
                    break;
                case 'r':
                    rotate(&t1);
                    break;
            }
            check_lines_multi(&t1, &t2);
        }
        c=15;
        while (c>0) {
            printf("\n\nGIOCATORE 2\n");
            print(&t2);
            cmd2 = chiediMossa();

            switch (cmd2) {
                case 'a':
                    t2.x--;
                    if (hittest(&t2))
                        t2.x++;
                    break;
                case 'd':
                    t2.x++;
                    if (hittest(&t2))
                        t2.x--;
                    break;
                case 's':
                    gravity(&t2);
                    c--;
                    break;
                case 'r':
                    rotate(&t2);
                    break;
            }
            check_lines_multi(&t2, &t1);
        }
    }


    print(&t1);
    print(&t2);
    if(t1.gameover==1)
        printf("GIOCATORE 1 : GAME OVER \n");
    else if(t2.gameover==1)
        printf("GIOCATORE 2 : GAME OVER \n");

    clean(&t1);
    clean(&t2);
}