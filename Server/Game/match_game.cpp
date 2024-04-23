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
void print_board(std::string board[NUM_ROWS][NUM_COLS], int selected1, int selected2) {
    std::cout << "   ";
    for (int col = 0; col < NUM_COLS; col++) {
        std::cout << col + 1 << " ";
    }
    std::cout << "\n";
    for (int row = 0; row < NUM_ROWS; row++) {
        std::cout << row + 1 << " |";
        for (int col = 0; col < NUM_COLS; col++) {
            std::string card = board[row][col];
            if (row * NUM_COLS + col == selected1 || row * NUM_COLS + col == selected2) {
                std::cout << card << " ";
            }
            else if (card == "x") {
                std::cout << card << " ";
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << "|\n";
    }
}

/******************************************************************************************************************************/
void print_rules(void) {
    std::cout << "The rules of the game are as follows:\n";
    std::cout << "1. The player should enter the locations of two cards.\n";
    std::cout << "2. The locations should be in numeric format (e.g., 11 for row 1, column 1).\n";
    std::cout << "3. If the location is outside the table range, an error message will be displayed.\n";
    std::cout << "4. If the selected cards match, they will be removed from the table.\n";
    std::cout << "5. If the selected cards do not match, an error message will be displayed.\n";
    std::cout << "6. The objective of the game is to match all the pairs in the least amount of turns possible.\n";
    std::cout << "7. The player cannot use any special characters as these will not be recognized by the game.\n";
}

/******************************************************************************************************************************/
void play_game(void) {
    char player_name[MAX_NAME_LEN];
    std::cout << "Please enter your name: ";
    std::cin >> player_name;
    std::cout << std::endl;

    // open file in append mode
    std::ofstream file("game_records.txt", std::ios::app);
    if (!file.is_open()) {
        std::cout << "Failed to open file." << std::endl;
        return;
    }

    // write player name to file
    file << "Player: " << player_name << std::endl;

    print_rules();
    std::cout << std::endl;

    char grid[NUM_CARDS] = { 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'd', 'e', 'e', 'f', 'f', 'g', 'g', 'h', 'h' };
    int high = NUM_CARDS - 1;
    int low = 0;

    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        int random = rand() % (high - low + 1) + low;
        int random2 = rand() % (high - low + 1) + low;
        char temp = grid[random];
        grid[random] = grid[random2];
        grid[random2] = temp;
    }

    std::string board[NUM_ROWS][NUM_COLS];
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            board[row][col] = grid[row * NUM_COLS + col];
        }
    }
    int num_turns = 0;
    int matched_pairs = 0;
    int selected1 = -1, selected2 = -1;

    while (matched_pairs < NUM_CARDS / 2) {
        std::cout << "\nTurn " << num_turns + 1 << ":" << std::endl;
        print_board(board, selected1, selected2);

        char row1, col1, row2, col2;
        std::cout << player_name << ", enter the location of the first card: ";
        std::cin >> row1 >> col1;

        int index1 = get_index(row1, col1);
        if (index1 == -1) {
            std::cout << "Invalid location. Please enter a valid location." << std::endl;
            continue;
        }

        std::cout << player_name << ", enter the location of the second card: ";
        std::cin >> row2 >> col2;

        int index2 = get_index(row2, col2);
        if (index2 == -1) {
            std::cout << "Invalid location. Please enter a valid location." << std::endl;
            continue;
        }

        if (index1 == index2) {
            std::cout << "You cannot select the same card twice. Please select a different card." << std::endl;
            continue;
        }

        std::string card1 = board[index1 / NUM_COLS][index1 % NUM_COLS];
        std::string card2 = board[index2 / NUM_COLS][index2 % NUM_COLS];

        if (card1 == "x" || card2 == "x") {
            std::cout << "You have already matched one of these cards. Please select a different card." << std::endl;
            continue;
        }

        if (card1 == card2) {
            std::cout << "Congratulations! You have matched a pair." << std::endl;
            board[index1 / NUM_COLS][index1 % NUM_COLS] = 'x';
            board[index2 / NUM_COLS][index2 % NUM_COLS] = 'x';
            matched_pairs++;
        }
        else {
            std::cout << "Sorry, the selected cards do not match." << std::endl;
        }

        selected1 = index1;
        selected2 = index2;
        num_turns++;
    }

    std::cout << "\nCongratulations " << player_name << ", you have won the game in " << num_turns << " turns!" << std::endl;

}

/******************************************************************************************************************************/
void play_again(void) {
    char play_again;
    while (true) {
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> play_again;
        if (tolower(play_again) == 'y') {
            std::cout << std::endl;
            play_game();
            break;
        }
        else if (tolower(play_again) == 'n') {
            std::cout << "Thanks for playing!" << std::endl;
            break;
        }
        else {
            std::cout << "Invalid input. Please enter y or n." << std::endl;
        }
    }
}
