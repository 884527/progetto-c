#ifndef PROGETTO_C_TETRIS_H
#define PROGETTO_C_TETRIS_H

struct tetris;

void init(struct tetris *t);

void clean(struct tetris *t);

void print(struct tetris *t);

int tocca(struct tetris *t);

void nuovo(struct tetris *t);

void nuovo_multi (struct tetris *t, struct tetris *t_avv);

void print_blocco(struct tetris *t);

int ruota(struct tetris *t);

int giu(struct tetris *t);

void elimina(struct tetris *t, int l);

void controllo_righe(struct tetris *t);

void controllo_multi (struct tetris *t_gio, struct tetris *t_avv);

void inverte(struct tetris *t, int n);

void run();

void run_multi();

#endif //PROGETTO_C_TETRIS_H
