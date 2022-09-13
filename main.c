#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"

int main () {
    int mod=0,exit=0;
    while(exit==0) {
        printf("Scegli la modalita' di gioco: \n1-single player \n2-multi player\n");
        scanf("%d",&mod);
        if (mod == 1) {
            run();
            exit = 1;
        }else if(mod==2) {
            run_multi();
            exit = 1;
        }else
            printf("\nNumero errato, riprova\n\n");
    }
    return 0;
}