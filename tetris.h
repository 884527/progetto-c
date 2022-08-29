#ifndef PROGETTO_C_TETRIS_H
#define PROGETTO_C_TETRIS_H

struct tetris;

void tetris_cleanup_io();

void tetris_signal_quit(int);

void tetris_set_ioconfig();

void init(struct tetris *t);

void clean(struct tetris *t);

void print(struct tetris *t);

void run(int width, int height);

void new_block(struct tetris *t);

void new_block(struct tetris *t);

void print_block(struct tetris *t);

void rotate(struct tetris *t);

void gravity(struct tetris *t);

void fall(struct tetris *t, int l);

void check_lines(struct tetris *t);



#endif //PROGETTO_C_TETRIS_H
