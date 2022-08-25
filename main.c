#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
int main () {
    struct tetris *t;
    init(&t);
    print(&t);
    return 0;
}