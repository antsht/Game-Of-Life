#include <stdio.h>

#include "game_of_life_engine.h"

int main(void) {
    int exit = 0;
    int curr_state[F_H][F_W] = {0};
    draw(curr_state, "", STD_DRAW_MODE);
    exit = input(curr_state);
    if (freopen("/dev/tty", "r", stdin) && !exit) {
        int prev_state[F_H + 2][F_W + 2] = {0};
        start_game(curr_state, prev_state);
    }
    clear_screen();
    printf("Bye!\n");
    return 0;
}
