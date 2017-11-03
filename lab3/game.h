#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <stdlib.h>

#define maxChar 13
#define size 3

char** initializeBoard();

char** swapBoard(char* reply, char** out);

char* sendBoard(char** board);

int checkBoard(char** board);

#endif