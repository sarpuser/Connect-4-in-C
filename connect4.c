#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants
#define RANDOM 0
#define AI 1
#define PLAYER1 1
#define PLAYER2 2

// Forward declarations of helper functions used in main
void display_board_ASCII (int board[][COLUMNS]);

// Start the simulation
int main() {	
	int board[ROWS][COLUMNS] = {{0}};
	srand(time(0));
	// int board[ROWS][COLUMNS] = {{ 1 , 2 , 1 , 2 , 0 , 0 , 0 },
    //     						{ 2 , 1 , 2 , 1 , 2 , 1 , 2 },
    //      						{ 1 , 2 , 1 , 2 , 1 , 2 , 1 },
    //      						{ 1 , 2 , 1 , 2 , 1 , 2 , 1 },
    //      						{ 2 , 1 , 2 , 1 , 2 , 1 , 2 },
    // 							{ 2 , 1 , 2 , 1 , 2 , 1 , 2 }};
	int move_index, winner;
	
	display_board_ASCII(board);

	while (1) {
		move_index = player_move(board);
		/* move_index = -1 if board full. Look at connect4_functions.c
		If board is full, move_index is -1. Since -1 is an invalid move,
		add_move_board returns 0 and so the game ends with a tie.			*/
		if (!add_move_board(board, move_index, PLAYER1)) {
			printf("Game is a tie!\n");
			return 0;
		}
		winner = check_winning(board, move_index);
		if (winner) {
			printf("Player %d wins!\n", winner);
			return 0;
		}

		move_index = computer_move(board, PLAYER2, RANDOM);
		// move_index = -1 if board full. Look at connect4_functions.c
		if (move_index >= 0) {
			printf("Computer moved in column %d\n", move_index + 1);
		}
		/* If board is full, move_index is -1. Since -1 is an invalid move,
		add_move_board returns 0 and so the game ends with a tie.			*/
		if (!add_move_board(board, move_index, PLAYER2)) {
			printf("Game is a tie!\n");
			return 0;
		}
		winner = check_winning(board, move_index);
		if (winner) {
			printf("Player %d wins!\n", winner);
			return 0;
		}
	}

	return 0;
}

