#include "game_of_life_engine.h"

#include <ncurses.h>
#include <stdio.h>

void start_game(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]) {
    init_ncurses();
    int speed = INITIAL_DELAY;
    int frame_counter = 0;
    long long generation = 0;
    int max_population = 0;
    char message[MESSAGE_BUFFER_LEN] = "";
    for (int i = MESSAGE_TIMEOUT; i > 0; --i) {
        sprintf(message, "The GAME OF LIFE will begin in %d seconds...", i);
        draw(curr_state, message, NCURSES_DRAW_MODE);
        napms(ONE_SECOND);
    }

    // main game loop
    while (1) {
        // skip idle steps
        if ((++frame_counter) % speed == 0) {
            // calculate stats
            int current_population = get_population(curr_state);
            if (current_population > max_population) max_population = current_population;
            sprintf(message, "Evoluting... Gen: %08lld Pop: %04d Max Pop: %04d Speed: %d | Press q to exit",
                    generation, current_population, max_population, MAX_DELAY - speed);
            draw(curr_state, message, NCURSES_DRAW_MODE);
            // copy current stat to a buffer for calculations
            copy_state(curr_state, prev_state);
            // calculate next generation
            life_cycle(curr_state, prev_state);
            generation++;
            // end game if there is no changes
            if (check_for_end(curr_state, prev_state)) {
                for (int i = MESSAGE_TIMEOUT; i > 0; --i) {
                    sprintf(message,
                            "There is no new life after %lld generation. Close in %d "
                            "seconds...",
                            generation, i);
                    draw(curr_state, message, NCURSES_DRAW_MODE);
                    napms(ONE_SECOND);
                }
                break;
            }
        }
        // interactive speed control
        char key_pressed = getch();
        napms(5);
        if (key_pressed == '+' && speed > MIN_DELAY) speed -= 1;
        if (key_pressed == '-' && speed < MAX_DELAY) speed += 1;
        if (key_pressed == 'q' || key_pressed == 'x') break;
    }
    endwin();
}

// if buffers are is equal - stop the game
int check_for_end(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]) {
    int stop = 1;
    for (int i = 1; i < F_H + 1; ++i)
        for (int j = 1; j < F_W + 1; ++j)
            if (curr_state[i - 1][j - 1] != prev_state[i][j]) stop = 0;
    return stop;
}

// get count of neighbours and calculate new state
void life_cycle(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]) {
    for (int i = 1; i < F_H + 1; ++i) {
        for (int j = 1; j < F_W + 1; ++j) {
            int neighbours = count_neighbours(prev_state, i, j);
            if ((prev_state[i][j] == 1 && neighbours == 2) || neighbours == 3)
                curr_state[i - 1][j - 1] = 1;
            else
                curr_state[i - 1][j - 1] = 0;
        }
    }
}

// count neighbours of current cell
int count_neighbours(int state_array[F_H + 2][F_W + 2], int i, int j) {
    int neighbours = 0;
    for (int k = -1; k < 2; ++k) {
        for (int l = -1; l < 2; ++l) {
            if (!(k == 0 && l == 0)) neighbours = neighbours + state_array[i + k][j + l];
        }
    }
    return neighbours;
}

// input of initial game state
int input(int state[F_H][F_W]) {
    char mode = 0;
    int exit = 0;
    do {
        draw(state, "Hello, user! Data input phase", STD_DRAW_MODE);
        printf("Press M to manual enter initial points | X to exit: ");
    } while (!(scanf("%c", &mode) == 1 &&
               (mode == 'm' || mode == 'M' || mode == 'a' || mode == 'A' || mode == 'x' || mode == 'X')));
    if (mode == 'm' || mode == 'M') {
        int col = 0, row = 0;
        draw(state, "Hello, user! Data input phase", STD_DRAW_MODE);
        printf(
            "Enter initial points (ROW COL -> enter)| any letter -> enter to "
            "begin simulation: \n");
        while (scanf("%d %d", &row, &col) == 2) {
            if ((col >= 1 && col <= F_W) && (row >= 1 && row <= F_H)) {
                state[row - 1][col - 1] = (state[row - 1][col - 1] + 1) % 2;
            }
            draw(state, "Hello, user! Data input phase", STD_DRAW_MODE);
            printf("Enter initial points | any letter to begin simulation: \n");
        }
    }
    if (mode == 'a' || mode == 'A') {
        for (int i = 0; i < F_H; ++i)
            for (int j = 0; j < F_W; ++j) scanf("%d", &state[i][j]);
    }
    if (mode == 'x' || mode == 'X') {
        exit = 1;
    }
    // clear stdin buffer
    flushinp();
    return exit;
}

// decorator over drawing functions in different terminal modes
void draw(int state[F_H][F_W], char *message, int mode) {
    switch (mode) {
        case STD_DRAW_MODE:
            draw_std(state, message);
            break;
        case NCURSES_DRAW_MODE:
            draw_ncurses(state, message);
            break;
    }
}

// draw in standart terminal mode
void draw_std(int state[F_H][F_W], char *message) {
    clear_screen();
    printf("%s\n ", message);
    for (int j = 0; j < F_W; ++j) printf("=");
    printf("\n");
    for (int i = 0; i < F_H; ++i) {
        printf("|");
        for (int j = 0; j < F_W; ++j) printf("%c", (state[i][j] == 0) ? DEAD_SYMBOL : LIVE_SYMBOL);
        printf("|\n");
    }
    printf(" ");
    for (int j = 0; j < F_W; ++j) printf("=");
    printf(" \n");
}

// draw in ncurses mode
void draw_ncurses(int state[F_H][F_W], char *message) {
    erase();
    printw("%s\n ", message);
    for (int j = 0; j < F_W; ++j) printw("=");
    printw("\n");
    for (int i = 0; i < F_H; ++i) {
        printw("|");
        for (int j = 0; j < F_W; ++j) printw("%c", (state[i][j] == 0) ? DEAD_SYMBOL : LIVE_SYMBOL);
        printw("|\n");
    }
    printw(" ");
    for (int j = 0; j < F_W; ++j) printw("=");
    printw("\n");
    refresh();
}

// copy current state to a buffer array for calculations
void copy_state(int source[F_H][F_W], int target[F_H + 2][F_W + 2]) {
    // copy main array
    for (int i = 0; i < F_H; ++i)
        for (int j = 0; j < F_W; ++j) target[i + 1][j + 1] = source[i][j];

    // copy opposite edges to create loop
    for (int j = 0; j < F_W; ++j) {
        target[0][j + 1] = source[F_H - 1][j];
        target[F_H + 1][j + 1] = source[0][j];
    }
    for (int i = 0; i < F_H; ++i) {
        target[i + 1][0] = source[i][F_W - 1];
        target[i + 1][F_W + 1] = source[i][0];
    }
    // copy diagonal edges
    target[0][0] = source[F_H - 1][F_W - 1];
    target[0][F_W + 1] = source[F_H - 1][0];
    target[F_H + 1][0] = source[0][F_W - 1];
    target[F_H + 1][F_W + 1] = source[0][0];
}

// count live cell in current state
int get_population(int state[F_H][F_W]) {
    int population = 0;
    for (int i = 0; i < F_H; ++i)
        for (int j = 0; j < F_W; ++j) population += state[i][j];
    return population;
}

// clear terminal in standart mode
void clear_screen() { printf("\033[2J\033[1;1H"); }

// switch to ncurses mode
void init_ncurses() {
    initscr();    // init ncusres structures
    savetty();    // save terminal params
    cbreak();     // no wait for enter after key press
    noecho();     // no echo of press keys to terminal
    timeout(0);   // get keys without delay
    curs_set(0);  // hide cursor symbol
}