#include <time.h>
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
#define maxGames 20
#define port 707


typedef struct Move
{
    int row, col;
} Move;

char** initializeBoard();
char** swapBoard(char* reply, char** out);
char* sendBoard(char** board);
int checkBoard(char** board);
int minimax(char** board, int depth, bool isMax);
bool isMovesLeft(char** board);
struct Move findBestMove(char** board, char player);
int uniform_distribution(int rangeLow, int rangeHigh);
char** setBoard(char** board);
int randInRange(int min, int max);
void write_server_int(int sockfd, int msg);

char player, opponent;
char empty = '-';

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    Move bestMove;
    bool first = true;
    char* serverReply = malloc(maxChar*sizeof(char));
    char* message = malloc(maxChar*sizeof(char));
    char** board = initializeBoard();
    srand((unsigned)time(NULL));
    int gameCounter = 0;

    player = 'X';
    opponent = 'O';

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        if (!isMovesLeft(board))
        {
            board = setBoard(board);
            first = true;
            puts(message);
            continue;
        }

        if (first)
        {
            gameCounter++;
            if (gameCounter > maxGames)
            {
                break;
            }
            // printf("Game # %d\n", gameCounter);
            bestMove.row = uniform_distribution(0,size-1);
            bestMove.col = uniform_distribution(0,size-1);
            printf("Row: %d , Col: %d\n", bestMove.row, bestMove.col);
            board[bestMove.row][bestMove.col] = player;
            message = sendBoard(board);
            first = false;
        }

        if( send(sock, message, maxChar+1 , 0) < 0)
        {
            //process first move here
            puts("Send failed");
            return 1;
        }

        printf("Kevin's %s\n", message);

        //Receive a reply from the server
        if(recv(sock , serverReply , maxChar , 0) < 0)
        {
            puts("Receive failed");
            break;
        }

        printf("Sherman's %s\n", serverReply);
        if (serverReply == "tie")
        {
            board = setBoard(board);
            first = true;
            // puts(message);
            continue;
        }
        board = swapBoard(serverReply, board);
        bestMove = findBestMove(board, player);
        board[bestMove.row][bestMove.col] = player;
        message = sendBoard(board);
    }

    close(sock);
    free(board);
    free(message);
    free(serverReply);
    return 0;
}

/*Initializes brand new board to empty*/
char** initializeBoard()
{
    char **board = malloc(size * sizeof(char *));
    for (int i = 0; i < size; ++i)
    {
        board[i] = malloc(size * sizeof(char));
    }

    // for (int row = 0; row < size; ++row)
    // {
    //  for (int col = 0; col < size; ++col)
    //  {
    //      board[row][col] = empty;
    //  }
    // }
    board = setBoard(board);
    return board;
}

/*Swaps the board after each message/reply*/
char** swapBoard(char* reply, char** board)
{
    int col = 0;
    int row = 0;
    for (int i = 0; i < maxChar; ++i)
    {
        if (row >= 3)
        {
            row = 0;
        }
        if (reply[i] == '|')
        {
            row++;
            col = 0;
            continue;
        }
        if (reply[i] == '\\')
        {
            break;
        }
        board[row][col] = reply[i];
        col++;
    }
    return board;
}


/* puts 2D board into a single message for writing*/
char* sendBoard(char** board)
{
    int count = 0;
    char* msg = malloc(size*size*sizeof(char) + sizeof(char));

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
    msg[count+2] = '\n';
    return msg;
}



/* Checks the board to determine if there is a winner. */
int checkBoard(char** board)
{
    // Checking for Rows for X or O victory.
    for (int row = 0; row<3; row++)
    {
        if (board[row][0]==board[row][1] &&
            board[row][1]==board[row][2])
        {
            if (board[row][0]==player)
                return +10;
            else if (board[row][0]==opponent)
                return -10;
        }
    }

    // Checking for Columns for X or O victory.
    for (int col = 0; col<3; col++)
    {
        if (board[0][col]==board[1][col] &&
            board[1][col]==board[2][col])
        {
            if (board[0][col]==player)
                return +10;

            else if (board[0][col]==opponent)
                return -10;
        }
    }

    // Checking for Diagonals for X or O victory.
    if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
    {
        if (board[0][0]==player)
            return +10;
        else if (board[0][0]==opponent)
            return -10;
    }

    if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
    {
        if (board[0][2]==player)
            return +10;
        else if (board[0][2]==opponent)
            return -10;
    }

    // Else if none of them have won then return 0
    return 0;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int minimax(char** board, int depth, bool isMax)
{
    int score = checkBoard(board);
    // If Maximizer has won the game return his/her
    // evaluated score
    if (score == 10)
        return score;

    // If Minimizer has won the game return his/her
    // evaluated score
    if (score == -10)
        return score;

    // If there are no more moves and no winner then
    // it is a tie
    if (isMovesLeft(board)==false)
        return 0;

    // If this maximizer's move
    if (isMax)
    {
        int best = -1000;

        // Traverse all cells
        for (int i = 0; i<size; i++)
        {
            for (int j = 0; j<size; j++)
            {
                // Check if cell is empty
                if (board[i][j]==empty)
                {
                    // Make the move
                    board[i][j] = player;

                    // Call minimax recursively and choose
                    // the maximum value
                    best = max( best,
                        minimax(board, depth+1, !isMax) );

                    // Undo the move
                    board[i][j] = empty;
                }
            }
        }
        return best;
    }

    // If this minimizer's move
    else
    {
        int best = 1000;

        // Traverse all cells
        for (int i = 0; i<size; i++)
        {
            for (int j = 0; j<size; j++)
            {
                // Check if cell is empty
                if (board[i][j]== empty)
                {
                    // Make the move
                    board[i][j] = opponent;

                    // Call minimax recursively and choose
                    // the minimum value
                    best = min(best,
                        minimax(board, depth+1, !isMax));

                    // Undo the move
                    board[i][j] = empty;
                }
            }
        }
        return best;
    }
}

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool isMovesLeft(char** board)
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]== empty)
                return true;
    return false;
}

// This will return the best possible move for the player
Move findBestMove(char** board, char player)
{
    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
 
    // Traverse all cells, evalutae minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i<size; i++)
    {
        for (int j = 0; j<size; j++)
        {
            // Check if cell is empty
            if (board[i][j] == empty)
            {
                // Make the move
                board[i][j] = player;
 
                // compute evaluation function for this
                // move.
                int moveVal = minimax(board, 0, false);
 
                // Undo the move
                board[i][j] = empty;
 
                // If the value of the current move is
                // more than the best value, then update
                // best/
                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    // printf("%d, %d\n", bestMove.row, bestMove.col);
    return bestMove;
}

/*Generates a uniformly random distribution from the specified
  range. The range is inclusive.
*/
int uniform_distribution(int rangeLow, int rangeHigh) 
{
    double myRand = rand()/(1.0 + RAND_MAX); 
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}

/*Helper extracted from initialize board*/
char** setBoard(char** board)
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            board[row][col] = empty;
        }
    }
    return board;
}
