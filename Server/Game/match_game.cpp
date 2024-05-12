#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <ctime>

#include "match_game.h"

/******************************************************************************************************************************/
int get_index(char row_char, char col_char) {
    int row = row_char - '1';
    int col = col_char - '1';

    if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS) {
        return -1;
    }
    return row * NUM_COLS + col;
}

/******************************************************************************************************************************/
std::string get_board(std::string board[NUM_ROWS][NUM_COLS], int selected1, int selected2) {
    std::string temp;
    temp += "   ";
    for (int col = 0; col < NUM_COLS; col++) {
        temp += std::to_string(col + 1) + " ";
    }
    temp += "\n";
    for (int row = 0; row < NUM_ROWS; row++) {
        temp += std::to_string(row + 1) + " |";
        for (int col = 0; col < NUM_COLS; col++) {
            std::string card = board[row][col];
            if (row * NUM_COLS + col == selected1 || row * NUM_COLS + col == selected2) {
                temp += card + " ";
            }
            else if (card == "x") {
                temp += card + " ";
            }
            else {
                temp += "  ";
            }
        }
        temp += "|\n";
    }
    return temp;
}

/******************************************************************************************************************************/
std::string get_rules(void) {
    std::string temp;

    temp += "-------------------------------------------------------------------------------------------------\n";
    temp += "| The rules of the game are as follows:                                                         |\n";
    temp += "| 1. The player should enter the locations of two cards.                                        |\n";
    temp += "| 2. The locations should be in numeric format (e.g., 11 for row 1, column 1).                  |\n";
    temp += "| 3. If the location is outside the table range, an error message will be displayed.            |\n";
    temp += "| 4. If the selected cards match, they will be removed from the table.                          |\n";
    temp += "| 5. If the selected cards do not match, an error message will be displayed.                    |\n";
    temp += "| 6. The objective of the game is to match all the pairs in the least amount of turns possible. |\n";
    temp += "| 7. The player cannot use any special characters as these will not be recognized by the game.  |\n";
    temp += "-------------------------------------------------------------------------------------------------\n";

    return temp;
}