//
// Created by cheyao on 22.12.2022.
// Copyright (c) 2022 cheyao All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define SNAKE_MAX_LENGTH ((BOARD_WIDTH - 2) * (BOARD_HEIGHT - 2))

#define BOARD_WIDTH 32
#define BOARD_HEIGHT 16

#define FOOD_COUNT 1
static const int game_speed = 100000;

static const int wrap = 1;
static const int snake_starting_length = 4;

static const int snake_starting_pos_x = 16;
static const int snake_starting_pos_y = 8;

static const int food_score_increment = 1;

static const int food_snake_length_increment = 1;

static const char snake_head_up_char = 'v';
static const char snake_head_down_char = '^';
static const char snake_head_left_char = '>';
static const char snake_head_right_char = '<';

static const char snake_tail_char = '*';

static const char food_char = '%';

static const char board_corner_char = '+';
static const char board_vertical_char = '|';
static const char board_horizontal_char = '-';
static const char board_blank_char = ' ';

static const char *color_red = "\033[31m";
static const char *color_green = "\033[32m";
static const char *color_yellow = "\033[33m";
static const char *color_blue = "\033[34m";
static const char *color_cyan = "\033[36m";
static const char *color_white = "\033[37m";

void initialize_game(void);

void finalize_game(void);

void game_input(void);

void game_update(void);

void game_draw(void);

#define CLEAR_SCREEN() printf("\033[2J")

#define RESET_CURSOR_POSITION() printf("\033[0;0H")

static char board[BOARD_HEIGHT][BOARD_WIDTH];
static int running;
static int paused;
static int snake_body_x[SNAKE_MAX_LENGTH], snake_body_y[SNAKE_MAX_LENGTH];
static int snake_length;
static char snake_direction;
static int food_x;
static int food_y;
static int score;

#define DEFAULT_CHAR_COLOR color_white

#define SNAKE_HEAD_CHAR_COLOR color_red
#define SNAKE_TAIL_CHAR_COLOR color_green

#define FOOD_CHAR_COLOR color_yellow

#define BOARD_CORNER_CHAR_COLOR color_cyan
#define BOARD_VERTICAL_CHAR_COLOR color_blue
#define BOARD_HORIZONTAL_CHAR_COLOR color_blue

#define DIRECTION_UP 'k'
#define DIRECTION_DOWN 'j'
#define DIRECTION_LEFT 'h'
#define DIRECTION_RIGHT 'l'

#define DEBUG_INCREASE_SNAKE_LENGTH_KEY 'g'

#define PAUSE_KEY 'p'

#define SNAKE_STARTING_DIRECTION DIRECTION_RIGHT

int kbhit(void) {
	char ch = ' '; // getchar();
	if (ch != EOF) {
		// ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

int randnum(int min, int max) {
	return (int) ((rand() % max) + min);
}

void initialize_game(void) {
	running = 1;
	score = 0;
	snake_direction = SNAKE_STARTING_DIRECTION;
	snake_length = snake_starting_length;
	for (int i = 0; i < snake_length; i++) {
		snake_body_x[i] = snake_starting_pos_x - i;
		snake_body_y[i] = snake_starting_pos_y;
	}
	food_x = randnum(1, BOARD_WIDTH - 2);
	food_y = randnum(1, BOARD_HEIGHT - 2);
}

void game_input(void) {
	if (kbhit()) {
		char ch = ' ';// getchar();
		if (ch == 'q') {
			running = 0;
		} else {
			switch (ch) {
				case DIRECTION_UP:
					if (snake_direction != DIRECTION_DOWN && !paused) {
						snake_direction = ch;
					}
					break;
				case DIRECTION_DOWN:
					if (snake_direction != DIRECTION_UP && !paused) {
						snake_direction = ch;
					}
					break;
				case DIRECTION_LEFT:
					if (snake_direction != DIRECTION_RIGHT && !paused) {
						snake_direction = ch;
					}
					break;
				case DIRECTION_RIGHT:
					if (snake_direction != DIRECTION_LEFT && !paused) {
						snake_direction = ch;
					}
					break;
				case PAUSE_KEY:
					paused ^= 1;
					break;
				default:
					break;
			}
		}
	}
}

void game_update(void) {
	for (int i = snake_length; i > 0; i--) {
		snake_body_x[i] = snake_body_x[i - 1];
		snake_body_y[i] = snake_body_y[i - 1];
	}
	switch (snake_direction) {
		case DIRECTION_UP:
			snake_body_y[0]--;
			break;
		case DIRECTION_DOWN:
			snake_body_y[0]++;
			break;
		case DIRECTION_LEFT:
			snake_body_x[0]--;
			break;
		case DIRECTION_RIGHT:
			snake_body_x[0]++;
		default:
			break;
	}
	for (int i = 1; i < snake_length; i++) {
		if ((snake_body_x[0] == snake_body_x[i]) && (snake_body_y[0] == snake_body_y[i])) {
			running = 0;
		}
	}
	switch (snake_body_x[0]) {
		case 0:
			if (wrap) {
				snake_body_x[0] = BOARD_WIDTH - 2;
			} else {
				running = 0;
			}
			break;
		case (BOARD_WIDTH - 1):
			if (wrap) {
				snake_body_x[0] = 1;
			} else {
				running = 0;
			}
		default:
			break;
	}
	switch (snake_body_y[0]) {
		case 0:
			if (wrap) {
				snake_body_y[0] = BOARD_HEIGHT - 2;
			} else {
				running = 0;
			}
			break;
		case (BOARD_HEIGHT - 1):
			if (wrap) {
				snake_body_y[0] = 1;
			} else {
				running = 0;
			}
		default:
			break;
	}
	if ((snake_body_x[0] == food_x) && (snake_body_y[0] == food_y)) {
		score += food_score_increment;
		snake_length += food_snake_length_increment;
		food_x = randnum(1, BOARD_WIDTH - 2);
		food_y = randnum(1, BOARD_HEIGHT - 2);
	}
}

void game_draw(void) {
	for (int j = 0; j < BOARD_HEIGHT; j++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			board[j][i] = board_blank_char;
		}
	}
	board[0][0] = board_corner_char;
	board[0][BOARD_WIDTH - 1] = board_corner_char;
	board[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] = board_corner_char;
	board[BOARD_HEIGHT - 1][0] = board_corner_char;
	for (int i = 1; i < BOARD_WIDTH - 1; i++) {
		board[0][i] = board_horizontal_char;
		board[BOARD_HEIGHT - 1][i] = board_horizontal_char;
	}
	for (int i = 1; i < BOARD_HEIGHT - 1; i++) {
		board[i][0] = board_vertical_char;
		board[i][BOARD_WIDTH - 1] = board_vertical_char;
	}
	switch (snake_direction) {
		case DIRECTION_UP:
			board[snake_body_y[0]][snake_body_x[0]] = snake_head_up_char;
			break;
		case DIRECTION_DOWN:
			board[snake_body_y[0]][snake_body_x[0]] = snake_head_down_char;
			break;
		case DIRECTION_LEFT:
			board[snake_body_y[0]][snake_body_x[0]] = snake_head_left_char;
			break;
		case DIRECTION_RIGHT:
			board[snake_body_y[0]][snake_body_x[0]] = snake_head_right_char;
		default:
			break;
	}
	for (int i = 1; i < snake_length; i++) {
		board[snake_body_y[i]][snake_body_x[i]] = snake_tail_char;
	}
	board[food_y][food_x] = food_char;
	RESET_CURSOR_POSITION();
	for (int j = 0; j < BOARD_HEIGHT; j++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (board[j][i] == snake_head_up_char || board[j][i] == snake_head_down_char ||
			    board[j][i] == snake_head_left_char || board[j][i] == snake_head_right_char) {
				printf("%s%c", SNAKE_HEAD_CHAR_COLOR, board[j][i]);
			} else if (board[j][i] == snake_tail_char) {
				printf("%s%c", SNAKE_TAIL_CHAR_COLOR, board[j][i]);
			} else if (board[j][i] == food_char) {
				printf("%s%c", FOOD_CHAR_COLOR, board[j][i]);
			} else if (board[j][i] == board_corner_char) {
				printf("%s%c", BOARD_CORNER_CHAR_COLOR, board[j][i]);
			} else if (board[j][i] == board_vertical_char) {
				printf("%s%c", BOARD_VERTICAL_CHAR_COLOR, board[j][i]);
			} else if (board[j][i] == board_horizontal_char) {
				printf("%s%c", BOARD_HORIZONTAL_CHAR_COLOR, board[j][i]);
			} else {
				printf("%s%c", DEFAULT_CHAR_COLOR, board[j][i]);
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("%sScore: %d\n\n\n\n", DEFAULT_CHAR_COLOR, score);
}

int snake(void) {
	CLEAR_SCREEN();
	initialize_game();
	do {
		game_input();
		if (!paused) {
			game_update();
		}
		game_draw();
		if (running) {
			sleep(game_speed);
		}
	} while (running);

	return 0;
}
