#include "connect4.h"
#include <stdio.h>
#include <stdlib.h>

// Forward function declarations for functions not in connect4.h
int find_first_empty_row (int board[][COLUMNS], int move_index);
int check_vertical (int board[][COLUMNS], int last_move_index);
int check_horizontal (int board[][COLUMNS], int last_move_index);
int check_positive_diagonal (int board[][COLUMNS], int last_move_col_index);
int check_negative_diagonal (int board[][COLUMNS], int last_move_col_index);
int board_full (int board[][COLUMNS]);
void display_board_ASCII (int board[][COLUMNS]);

// Checks whether 'move' is valid given the current state of 'board'.
// It returns 1 when the move is valid, otherwise 0.
int valid_move(int board[][COLUMNS], int move_index) {
	if (move_index < 0 ||
		move_index > COLUMNS - 1 ||
		find_first_empty_row(board, move_index) == -1) {

		return 0;
	}
	else return 1;
}


// Update the board with a move for a specific player.
// Returns 0 in case of failure (the move is not a valid
// one); otherwise it returns 1. 
int add_move_board(int board[][COLUMNS], int move_index, int player) {
	if (!valid_move(board, move_index)) return 0;
	else {
		int first_empty_row = find_first_empty_row(board, move_index);
		board[first_empty_row][move_index] = player;
		display_board_ASCII(board);
		return 1;
	}
}


// It asks the user to pick a column to play, and repeats
// this until a valid one is chosen.
// The user should enter a number between 1 and COLUMNS (1-indexed).
// It returns the column in the board array, so a value between 
// 0 and COLUMNS-1 (0-indexed)
int player_move(int board[][COLUMNS]) {
	int move, move_index = -1;
	// If board is full, don't get a move and return -1.
	if (!board_full(board)) {
		while (!valid_move(board, move_index)) {
			printf("Please enter a move between %d and %d: ", 1, COLUMNS);
			scanf("%d", &move);
			while (getchar() != '\n');
			move_index = move - 1;
		}
	}
	return move_index;
}


// This function chooses a move for the computer player,
// given the state of the board and the player number the
// computer is playing with.
// It supports two modes: 0 for making random valid moves
// and 1 for using a more advanced algorithm.
// It returns the move chosen (a value between 0 an COLUMNS-1).
int computer_move(int board[][COLUMNS], int player, int mode) {
	int move_index = -1;
	// If board is full, don't get a move and return -1.
	if (!board_full(board)) {
		if (mode == 0) {
			while (!valid_move(board, move_index)) {
				move_index = rand() % (COLUMNS);
			}
		}
	}
	return move_index;
}	


// Checks whether there is a winning player on the board.
// It gets passed the last move that was made.
// It returns 0 if there is no winner, or the otherwise the number
// of the player who won (1 or 2).
int check_winning(int board[][COLUMNS], int last_move_index) {
	if (check_vertical(board, last_move_index) ||
		check_horizontal(board, last_move_index) ||
		check_negative_diagonal(board, last_move_index) ||
		check_positive_diagonal(board, last_move_index)) {

		int last_row = find_first_empty_row(board, last_move_index) + 1;
		return board[last_row][last_move_index];
	}
	else return 0;
}

/* This function starts from the bottom of the column that the last move was
placed in. It then moves up until it encounters a blank spot. It returns
the row index of the row with the blank spot. If the for loop reaches the 
top without breaking, this means that the column is full. In this case, 
this function returns -1.													*/
int find_first_empty_row (int board[][COLUMNS], int move_index) {
	int empty_row_number;
	for (int i = ROWS - 1; i >= 0; i--) {
		if (!board[i][move_index]) {
			empty_row_number = i;
			break;
		}
		else if (i == 0) empty_row_number = -1;
	}
	return empty_row_number;
}

/* This function finds the topmost row that a stone has been placed on in the
last move column. It does this by finding the first blank space in the
last move column using the the find_first_empty_row function and then
incrementing the row index by one (since the row indexes increase downwards).
The function then checks all the stones below the last move until either
it counts 4 of the same kind, or hits the opposite kind. If 4 of a kind is
found, this function returns 1, otherwise it returns 0.						*/
int check_vertical (int board[][COLUMNS], int last_move_index) {
	int count = 1;
	int last_move_row = find_first_empty_row(board, last_move_index) + 1;
	int last_move = board[last_move_row][last_move_index];

	for (int i = last_move_row + 1; i < ROWS; i++) {
		if (board[i][last_move_index] == last_move) count++;
		else if (board[i][last_move_index] != last_move || count == 4) break;
	}

	if (count == 4) return 1;
	else return 0;
}

/* This function finds the topmost row that a stone has been placed on in the
last move column. It does this by finding the first blank space in the
last move column using the the find_first_empty_row function and then
incrementing the row index by one (since the row indexes increase downwards).
The function then checks all the stones to right and left of the last move
until either it counts 4 of the same kind, or hits the opposite kind. If 4 of a
kind is found, this function returns 1, otherwise it returns 0.				*/
int check_horizontal (int board[][COLUMNS], int last_move_index) {
	int count = 1;
	int last_move_row = find_first_empty_row(board, last_move_index) + 1;
	int last_move = board[last_move_row][last_move_index];
	
	int new_col_index = last_move_index + 1;
	while (new_col_index < COLUMNS) {
		if (board[last_move_row][new_col_index++] == last_move) count++;
		else break;
	}

	new_col_index = last_move_index - 1;
	while (new_col_index >= 0) {
		if (board[last_move_row][new_col_index--] == last_move) count++;
		else break;
	}

	if (count >= 4) return 1;
	else return 0;
}

/* This function finds the topmost row that a stone has been placed on in the
last move column. It does this by finding the first blank space in the
last move column using the the find_first_empty_row function and then
incrementing the row index by one (since the row indexes increase downwards).
It then counts all the identical stones along the positive diagonal (bottom
left to top right) by first incrementing the column index and decrementing
the row index, and then by doing the opposite starting from the original
point until either it counts 4 of the same kind, or encounters the
opposite kind. If the count reaches to 4, the function returns 1, otherwise
the function returns 0.														*/
int check_positive_diagonal (int board[][COLUMNS], int last_move_col_index) {
	int count = 1;
	int last_move_row = find_first_empty_row(board, last_move_col_index) + 1;
	int last_move = board[last_move_row][last_move_col_index];

	int new_col_index = last_move_col_index + 1;
	int new_row_index = last_move_row - 1;	
	while (new_col_index < COLUMNS && new_row_index >= 0) {
		if (board[new_row_index--][new_col_index++] == last_move) count++;
		else break;
	}
	
	new_col_index = last_move_col_index - 1;
	new_row_index = last_move_row + 1;	
	while (new_col_index >= 0 && new_row_index < ROWS) {
		if (board[new_row_index++][new_col_index--] == last_move) count++;
		else break;
	}

	if (count >= 4) return 1;
	else return 0;
}

/* This function finds the topmost row that a stone has been placed on in the
last move column. It does this by finding the first blank space in the
last move column using the the find_first_empty_row function and then
incrementing the row index by one (since the row indexes increase downwards).
It then counts all the identical stones along the negative diagonal (bottom
right to top left) by first incrementing both the column index and the row
index, and then by doing the opposite starting from the original
point until either it counts 4 of the same kind, or encounters the
opposite kind. If the count reaches to 4, the function returns 1, otherwise
the function returns 0.														*/
int check_negative_diagonal (int board[][COLUMNS], int last_move_col_index) {
	int count = 1;
	int last_move_row = find_first_empty_row(board, last_move_col_index) + 1;
	int last_move = board[last_move_row][last_move_col_index];

	int new_col_index = last_move_col_index + 1;
	int new_row_index = last_move_row + 1;	
	while (new_col_index < COLUMNS && new_row_index < ROWS) {
		if (board[new_row_index++][new_col_index++] == last_move) count++;
		else break;
	}
	
	new_col_index = last_move_col_index - 1;
	new_row_index = last_move_row - 1;	
	while (new_col_index >= 0 && new_row_index >= 0) {
		if (board[new_row_index--][new_col_index--] == last_move) count++;
		else break;
	}

	if (count >= 4) return 1;
	else return 0;
}

/* This function checks all the columns to see if they are full or not.
If even one of the columns is not full, the function returns 0. Otherwise,
the function returns 1.														*/
int board_full (int board[][COLUMNS]) {
	for (int i = 0; i < COLUMNS; i++) {
		if (valid_move(board, i)) return 0;
	}
	return 1;
}