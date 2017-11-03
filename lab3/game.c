#include "game.h"

char** initializeBoard()
{
	char **board = malloc(size * sizeof(char *));
	for (int i = 0; i < size; ++i)
	{
		board[i] = malloc(size * sizeof(char));
	}

	for (int row = 0; row < size; ++row)
	{
		for (int col = 0; col < size; ++col)
		{
			board[row][col] = '-';
		}
	}
	return board;
}

char** swapBoard(char* reply, char** board)
{
	int col = 0;
	for (int i = 0; i < size*size; ++i)
	{
		board[i][col] = reply[i];
		if(i % size == 0) col++;
	}
	return board;
}


/* Draws the game board to stdout. */
char* sendBoard(char** board)
{
	int count = 0;
	char* msg = malloc(size*size*sizeof(char));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			msg[count] = board[i][j];
			count++;
		}
		if(count < size*size) 
		{
			msg[count] = '|';
			count++;
		}
	}
	msg[count] = '\\';
	msg[count+1] = 'n';
	return msg;
}



/* Checks the board to determine if there is a winner. */
int checkBoard(char** board)
{
	for (int row = 0; row < size; ++row)
	{
		for (int col = 0; col < size; ++col)
		{
			if ( board[row][0] == board[row][1] && board[row][1] == board[row][2] ) 
            { /* Check the row for a win. */
				return 1;
		}
		else if ( board[0][col] == board[1][col] && board[1][col] == board[2][col] ) 
            { /* Check the column for a win. */
			return 1;
	}
}
}

if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
{
	return 1;
}
else if (board[2][0] == board[1][1] && board[1][1] == board[0][2])
{
	return 1;
}
else return 0;
}