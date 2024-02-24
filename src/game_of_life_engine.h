#ifndef GAME_OF_LIFE_ENGINE_H
#define GAME_OF_LIFE_ENGINE_H

#define F_H 25  // Field height
#define F_W 80  // Field width
#define LIVE_SYMBOL 'o'
#define DEAD_SYMBOL '.'
#define STD_DRAW_MODE 1
#define NCURSES_DRAW_MODE 2
#define INITIAL_DELAY 50        // initial speed
#define MAX_DELAY 100           // lowest speed
#define MIN_DELAY 2             // max speed
#define MESSAGE_BUFFER_LEN 100  // buffer size for messages
#define ONE_SECOND 1000         // one second in ms
#define MESSAGE_TIMEOUT 5       // timeout when show messages

void draw(int state[F_H][F_W], char *message, int draw_mode);
void draw_std(int state[F_H][F_W], char *message);
void init_ncurses(void);
void draw_ncurses(int state[F_H][F_W], char *message);
int input(int state[F_H][F_W]);
void start_game(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]);
void copy_state(int source[F_H][F_W], int target[F_H + 2][F_W + 2]);
void life_cycle(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]);
int count_neighbours(int state_array[F_H + 2][F_W + 2], int i, int j);
int check_for_end(int curr_state[F_H][F_W], int prev_state[F_H + 2][F_W + 2]);
int get_population(int state[F_H][F_W]);
void clear_screen();

#endif