#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

int bool=0;

struct tetris {
    char **campo;
    int wd;     /** larghezza */
    int hw;     /** altezza */
    int punti;
    int gameover;
    int x;
    int y;

    struct blocco {
        char data[5][5];
        int wd;     /** larghezza */
        int hg;     /** altezza */
        int n;      /** numero di blocchi */
    } corr;
};

struct blocco bloc[] =
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

#define TETRAMINI (sizeof(bloc)/sizeof(struct blocco))

/**
 * inizializza il campo di gioco
 */
void init (struct tetris *t) {
    t->punti = 0;
    t->wd = 10;
    t->hw = 15;
    t->campo = malloc(sizeof(char *) * t->wd);
    t->gameover=0;
    int x, y;
    for(x=0;x<10;x++) {
        t->campo[x] = malloc(sizeof(char) * t->hw);
        for (y = 0; y < 15; y++)
            t->campo[x][y] = ' ';
    }
}

/**
 * pulisce il campo di gioco
 */
void clean (struct  tetris *t) {
    int x;
    for(x=0;x<t->wd; x++)
        free(t->campo[x]);
    free(t->campo);
}

/**
 * stampa il campo di gioco e il punteggio
 */
void print (struct tetris *t) {
    int x, y;
    printf("[SCORE: %d]\n", t->punti);
    for(x=0;x< t->wd + 2; x++)
        printf("~");
    printf("\n");

    for(y=0;y<t->hw; y++) {
        printf("|");
        for(x=0;x<t->wd; x++)
            if(x>=t->x && y>=t->y
               && x<(t->x + t->corr.wd) && y < (t->y + t->corr.hg)
               && t->corr.data[y - t->y][x - t->x] != ' ')
                printf("%c", t->corr.data[y - t->y][x - t->x]);
            else
                printf("%c", t->campo[x][y]);
        printf("|\n");
    }

    for(x=0;x< t->wd + 2; x++)
        printf("~");
    printf("\n");
}

/**
 * controlla quando il blocco corrente tocca gli altri blocchi nel campo oppure i confini del campo
 */
int tocca (struct tetris *t) {
    int x, y, xx, yy;
    struct blocco b = t->corr;
    for(x=0;x<b.wd;x++) {
        for(y=0;y<b.hg;y++) {
            xx = t->x +x;
            yy = t->y +y;
            if(xx<0 || xx>=t->wd)
                return 1;
            /** controllo se c'è un altro blocco */
            if(b.data[y][x]!=' ' && (yy>=t->hw || (xx < t->wd && t->campo[xx][yy] != ' ')))
                return 1;
        }
    }
    return 0;
}

/**
 * genera un nuovo blocco (in modo random) controllando il numero di blocchi: 20 blocchi per ogni tipo
 */
void nuovo (struct tetris *t) {
    int exit=0, x, tot=20*5;
    while(exit==0 && tot>0) {
        x = rand() % TETRAMINI;
        if (bloc[x].n > 0) {
            t->corr = bloc[x];
            bloc[x].n--;
            tot--;
            exit=1;
        }
    }
    t->x = (t->wd / 2) - (t->corr.wd / 2); //centro
    t->y=0; //prima riga

    if(tocca(t))
        t->gameover=1;
    if(tot==0) {
        t->gameover=1;
        printf("Hai terminato i blocchi!\n");
    }
}

/**
 * stampa il blocco
 */
void print_blocco (struct tetris *t) {
    int x, y;
    struct blocco b = t->corr;
    for(x=0;x<b.wd;x++) {
        for(y=0;y<b.hg;y++) {
            if(b.data[y][x]!=' ')
                t->campo[t->x + x][t->y + y] = b.data[y][x];
        }
    }
}

/**
 * fa scendere il blocco
 */
int giu (struct tetris *t) {
    int x, y;
    t->y+=1;
    if(tocca(t)) {
        t->y--;
        print_blocco(t);
        nuovo(t);
        return !bool;
    }
    return bool;
}

/**
 * ruota il blocco di 90 gradi
 */
int ruota (struct tetris *t) {
    struct blocco b=t->corr;
    struct blocco s=b;
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
    t->corr=b;
    if (tocca(t)) {
        t->corr=s;
        t->x=x;
        t->y=y;
        return !bool;
    }
    return bool;
}

/**
 * elimina la riga completa e fa scendere le righe superiori
 */
void elimina (struct tetris *t, int l) {
    int x, y;
    for(y=l;y>0;y--) {
        for(x=0;x<t->wd; x++)
            t->campo[x][y] = t->campo[x][y - 1];
    }
    for(x=0;x<t->wd; x++)
        t->campo[x][0] = ' ';
}

/**
 * controlla se ci sono righe complete da eliminare e nel caso incrementa il punteggio
 */
void controllo_righe (struct tetris *t) {
    int x, y, l, count=0;
    int p=1;
    for(y= t->hw - 1; y >= 0; y--) {
        l=1;
        for(x=0;x<t->wd && l; x++) {
            if (t->campo[x][y] == ' ') {
                l = 0;
                count=0;
            }
        }
        if (l) {
            count++;
            t->punti += p;
            if (count == 2)    /**quando si eliminano 3 righe*/
                p = 3;
            else if (count == 3)     /**quando si eliminano 4 righe*/
                p = 6;
            else
                p *= 2;       /** quando si eliminano 1 o 2 righe */
            elimina(t, y);
            y++;
        }
    }
}

/**
 * funzione per chiedere la mossa al giocatore
 */
char chiediMossa()  {
    char cmd;
    printf("Scegli la tua mossa: \nhw = sinistra \nd = destra \ns = giu' \nr = ruota hw destra \n\n");
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
    nuovo(&t);
    while (t.gameover==0) {
        print(&t);
        cmd=chiediMossa();

        switch (cmd) {
            case 'a':
                t.x--;
                if (tocca(&t))
                    t.x++;
                break;
            case 'd':
                t.x++;
                if (tocca(&t))
                    t.x--;
                break;
            case 's':
                giu(&t);
                break;
            case 'r':
                ruota(&t);
                break;
        }
        controllo_righe(&t);
    }

    print(&t);
    printf(" GAME OVER \n");

    clean(&t);
}


/**
 *     ***MODALITA' MULTI PLAYER***
 */

/**
 * genera un nuovo blocco (random), controllando se ci sono ancora blocchi disponibili
 */
void nuovo_multi (struct tetris *t, struct tetris *t_avv){
    int exit=0, x, tot=40*5;
    while(exit==0 && tot>0) {
        x = rand() % TETRAMINI;
        if (bloc[x].n > 0) {
            t->corr = bloc[x];
            bloc[x].n--;
            tot--;
            exit=1;
        }
    }
    t->x = (t->wd / 2) - (t->corr.wd / 2); //centro
    t->y=0; //prima riga

    if(tocca(t))
        t->gameover=1;
    if(tot==0) {
        printf("Blocchi terminati!\n");
        if(t->punti >= t_avv->punti)
            t_avv->gameover=1;
        else
            t->gameover=1;
    }
}

/**
 * controlla se ci sono righe complete da eliminare e in caso aumenta il punteggio o modifica il campo dell'avversario
 * t_gio è il giocatore, t_avv è l'avversario
 */
void controllo_multi (struct tetris *t_gio, struct tetris *t_avv) {
    int x, y, l, count=0;
    int p=1;
    for(y= t_gio->hw - 1; y >= 0; y--) {
        l=1;
        for(x=0;x<t_gio->wd && l; x++) {
            if (t_gio->campo[x][y] == ' ') {
                l = 0;
                count=0;
            }
        }
        if (l) {
            count++;
            t_gio->punti += p;
            if (count == 2) {    /**quando si eliminano 3 righe*/
                p = 3;
                inverte(&t_avv, count + 1);
            } else if (count == 3) {    /**quando si eliminano 4 righe*/
                p = 6;
                inverte(&t_avv, count + 1);
            }else
                p *= 2;       /** quando si eliminano 1 o 2 righe */
            elimina(t_gio, y);
            y++;
        }
    }
}

/**
 * inverte le ultime n righe del campo t: una posizione vuota diventa piena e viceversa
 */
void inverte(struct tetris *t, int n) {
    int x, y;
    for(y= t->hw - 1; y > t->hw - n; y--) {
        for(x=0;x<t->wd; x++) {
            if(t->campo[x][y] == ' ')
                t->campo[x][y] = 'X';
            else if(t->campo[x][y] != ' ')
                t->campo[x][y] = ' ';
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
    nuovo_multi(&t1, &t2);
    nuovo_multi(&t2, &t1);

    while (t1.gameover==0 && t2.gameover==0) {
        while (0==bool) {
            printf("\nGIOCATORE 1\n");
            print(&t1);
            cmd1 = chiediMossa();

            switch (cmd1) {
                case 'a':
                    t1.x--;
                    if (tocca(&t1)){
                        bool=2;
                        t1.x++;
                    }
                    break;
                case 'd':
                    t1.x++;
                    if (tocca(&t1)) {
                        t1.x--;
                        bool = 2;
                    }
                    break;
                case 's':
                    bool= giu(&t1);
                    printf("%d",bool);
                    break;
                case 'r':
                    ruota(&t1);
                    break;
            }
            controllo_multi(&t1, &t2);
        }

        while (1==bool) {
            printf("\n\nGIOCATORE 2\n");
            print(&t2);
            cmd2 = chiediMossa();

            switch (cmd2) {
                case 'a':
                    t2.x--;
                    if (tocca(&t2)) {
                        t2.x++;
                        bool = 1;
                    }
                    break;
                case 'd':
                    t2.x++;
                    if (tocca(&t2)) {
                        t2.x--;
                        bool = 1;
                    }
                    break;
                case 's':
                    bool= giu(&t2);
                    printf("%d",bool);
                    break;
                case 'r':
                    ruota(&t2);
                    break;
            }
            controllo_multi(&t2, &t1);
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