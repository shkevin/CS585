#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define maxChar 13
#define size 3

typedef struct Move
{
	int row, col;
} Move;

char player, opponent;

char** initializeBoard();

char** swapBoard(char* reply, char** out);

char* sendBoard(char** board);

int checkBoard(char** board);

int minimax(char** board, int depth, bool isMax);

bool isMovesLeft(char** board);

struct Move findBestMove(char** board);

int uniform_distribution(int rangeLow, int rangeHigh);

#endif