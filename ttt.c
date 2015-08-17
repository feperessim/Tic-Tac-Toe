/* Copyright (c) 2015, Felipe de Lima Peressim
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define BOARD_MAX_SIZE 9

typedef int strategy_fn (char [], char);
int win(char board[BOARD_MAX_SIZE], char player) 
{
	
	unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
	int i;

	for(i = 0; i < 8; ++i) {
		if(board[wins[i][0]] != 0 &&
			board[wins[i][0]] == board[wins[i][1]] &&
			board[wins[i][0]] == board[wins[i][2]]) {
			if  (board[wins[i][2]] == player)
				return 1;
			else if (board[wins[i][2]] != player)
				return -1;
		}
	}
	return 0;
}
void draw(char board[])
{
	int i;

	putchar('\n');
	for (i = 0; i < BOARD_MAX_SIZE; i+=3) {
		printf(" %c | %c | %c \n", (board[i] == 0) ? ' ' : board[i], (board[i+1] == 0) ? ' ' : board[i+1], (board[i+2] == 0) ? ' ' : board[i+2]);
               	if (i != 6)
			printf("---*---*---\n");
	}
	return;
}
void fill(char board[])
{
	int i;

	for (i = 0; i < BOARD_MAX_SIZE; ++i) {
		board[i] = 0;
	}
	return;
}
void copy(char from[], char to[])
{
	int i;

	for (i = 0; i < BOARD_MAX_SIZE; ++i) {
		to[i] = from[i];
	}
	return;
}
int end_game(char board[], char player)
{
        if (win(board, player) != 0)
                return 1;
        else {
                int i;

                for (i = 0; i < BOARD_MAX_SIZE; ++i) {
                        if (board[i] == 0) {
                                return 0;
                        }
                }
        }
        return 1;
}
int is_a_valid_move(char move)
{
        if (move == 0)
                return 1;
        else
                return 0;
}
int opponent (char player)
{
  return (player == 'x') ? 'o': 'x';
}
int minimax(char board[], char player) 
{
	if (end_game(board, player))
		return win(board, player);

	int i, score = 0, best_score = -1;
	char new_board[BOARD_MAX_SIZE];
	score = win(board, player);
	for (i = 0; i < BOARD_MAX_SIZE; ++i) {
                copy(board, new_board);
		if (is_a_valid_move(new_board[i])) {
			new_board[i] = player;
                        score = minimax(new_board, opponent(player)) * -1;
                       if (score > best_score) {
                                best_score = score;
			}
		}
	}
	return best_score;
}
int minimax_strategy(char board[], char player)
{
	char new_board[BOARD_MAX_SIZE];
	int best_move, score = 0, best_score = -1, i;

	for (i = 0; i < BOARD_MAX_SIZE; ++i) {
                copy(board, new_board);
		if (is_a_valid_move(new_board[i])) {
			new_board[i] = player;
			score = minimax(new_board, opponent(player)) * -1;
			if (score > best_score) {
				best_score = score;
				best_move = i;
			}
		}
	}
	return best_move;	
}
int random_strategy(char board[], char player)
{
	int move = rand() % 9;

	while (!(is_a_valid_move(board[move]))) {
		move = rand() % 9;
	}
	return move;
}
int human(char board[], char player)
{ 
	char move;
	
	move = getchar();
	while (!(isdigit(move))) {
		move = getchar();
	}
	return move -'0';
}
int ttt(char board[], strategy_fn strategy_x, strategy_fn strategy_o)
{
	int player = 'x', move;

	fill(board);
	draw(board);
	while (!(end_game(board, player))) {
		move = (player == 'x') ? strategy_x(board, player) : strategy_o(board, player);
		if ((move >= 0 && move <= 8) && (is_a_valid_move(board[move]))) {
			board[move] = player;
			draw(board);
			player = opponent(player);
		}
	}
	int w = win (board, 'x');
        if (w == 1)
          puts ("\nX WIN\n");
        else if (w == -1)
          puts ("\nO WIN\n");
        else 
          puts ("\nDRAW\n");
	return win(board, player);
}
int main(void)
{
	char board[BOARD_MAX_SIZE];
	
	srand(time(NULL));
	ttt(board, minimax_strategy, human);
	return 0;
}	
