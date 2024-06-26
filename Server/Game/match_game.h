#pragma once

#define NUM_CARDS 16
#define NUM_ROWS 4
#define NUM_COLS 4
#define MAX_NAME_LEN 30

#include <string>

// Prototype
int get_index(char row_char, char col_char);
std::string get_board(std::string board[NUM_ROWS][NUM_COLS], int selected1, int selected2);
std::string get_rules(void);