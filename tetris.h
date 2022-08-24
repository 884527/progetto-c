//
// Created by Martina Feltre on 23/08/2022.
//

#ifndef PROGETTO_C_TETRIS_H
#define PROGETTO_C_TETRIS_H

struct tetris;

void tetris_cleanup_io();

void tetris_signal_quit(int);

void tetris_set_ioconfig();

void init(struct tetris *t,int w,int h);

void clean(struct tetris *t);

void print(struct tetris *t);

void tetris_run(int width, int height);

void tetris_new_block(struct tetris *t);

void tetris_new_block(struct tetris *t);

void tetris_print_block(struct tetris *t);

void rotate(struct tetris *t);

void tetris_gravity(struct tetris *t);

void tetris_fall(struct tetris *t, int l);

void check_lines(struct tetris *t);



#endif //PROGETTO_C_TETRIS_H
